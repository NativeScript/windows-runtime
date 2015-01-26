#include "Metadata-Prefix.h"
#include "Signature.h"

namespace NativeScript {
namespace Metadata {
    using namespace std;

    PCCOR_SIGNATURE Signature::consumeType(PCCOR_SIGNATURE& signature) {
        PCCOR_SIGNATURE start = signature;

        CorElementType elementType{ CorSigUncompressElementType(signature) };
        switch (elementType) {
        case ELEMENT_TYPE_VOID:
        case ELEMENT_TYPE_BOOLEAN:
        case ELEMENT_TYPE_CHAR:
        case ELEMENT_TYPE_I1:
        case ELEMENT_TYPE_U1:
        case ELEMENT_TYPE_I2:
        case ELEMENT_TYPE_U2:
        case ELEMENT_TYPE_I4:
        case ELEMENT_TYPE_U4:
        case ELEMENT_TYPE_I8:
        case ELEMENT_TYPE_U8:
        case ELEMENT_TYPE_R4:
        case ELEMENT_TYPE_R8:
        case ELEMENT_TYPE_STRING:
            return start;

        case ELEMENT_TYPE_VALUETYPE:
            CorSigUncompressToken(signature);
            return start;

        case ELEMENT_TYPE_CLASS:
            CorSigUncompressToken(signature);
            return start;

        case ELEMENT_TYPE_OBJECT:
            return start;

        case ELEMENT_TYPE_SZARRAY:
            consumeType(signature);
            return start;

        case ELEMENT_TYPE_VAR:
            CorSigUncompressData(signature);
            return start;

        case ELEMENT_TYPE_GENERICINST: {
            CorSigUncompressElementType(signature);
            CorSigUncompressToken(signature);

            ULONG genericArgumentsCount{ CorSigUncompressData(signature) };
            for (size_t i = 0; i < genericArgumentsCount; ++i) {
                consumeType(signature);
            }

            return start;
        }

        case ELEMENT_TYPE_BYREF:
            consumeType(signature);
            return start;

        default:
            ASSERT_NOT_REACHED();
        }
    }

    void Signature::toString(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature, wstring& result) {
        CorElementType elementType{ CorSigUncompressElementType(signature) };
        switch (elementType) {
        case ELEMENT_TYPE_VOID:
            result += L"Void";
            break;

        case ELEMENT_TYPE_BOOLEAN:
            result += L"Boolean";
            break;

        case ELEMENT_TYPE_CHAR:
            result += L"Char16";
            break;

        case ELEMENT_TYPE_I1:
            result += L"Int8";
            break;

        case ELEMENT_TYPE_U1:
            result += L"UInt8";
            break;

        case ELEMENT_TYPE_I2:
            result += L"Int16";
            break;

        case ELEMENT_TYPE_U2:
            result += L"UInt16";
            break;

        case ELEMENT_TYPE_I4:
            result += L"Int32";
            break;

        case ELEMENT_TYPE_U4:
            result += L"UInt32";
            break;

        case ELEMENT_TYPE_I8:
            result += L"Int64";
            break;

        case ELEMENT_TYPE_U8:
            result += L"UInt64";
            break;

        case ELEMENT_TYPE_R4:
            result += L"Single";
            break;

        case ELEMENT_TYPE_R8:
            result += L"Double";
            break;

        case ELEMENT_TYPE_STRING:
            result += L"String";
            break;

        case ELEMENT_TYPE_VALUETYPE: {
            mdToken token{ CorSigUncompressToken(signature) };
            identifier className = getClassName(metadata, token);
            if (wcscmp(className.data(), L"System.Guid") == 0) {
                result += L"Guid";
            } else {
                result += className.data();
            }
            break;
        }

        case ELEMENT_TYPE_CLASS: {
            mdToken token{ CorSigUncompressToken(signature) };
            result += getClassName(metadata, token).data();
            break;
        }

        case ELEMENT_TYPE_OBJECT:
            result += L"Object";
            break;

        case ELEMENT_TYPE_SZARRAY:
            toString(metadata, signature, result);
            result += L"[]";
            break;

        case ELEMENT_TYPE_VAR: {
            ULONG index{ CorSigUncompressData(signature) };
            result += L"Var!";
            result += to_wstring(index);
            break;
        }

        case ELEMENT_TYPE_GENERICINST: {
            CorElementType genericType{ CorSigUncompressElementType(signature) };
            ASSERT(genericType == ELEMENT_TYPE_CLASS);

            mdToken token{ CorSigUncompressToken(signature) };
            result += getClassName(metadata, token).data();

            result += L'<';

            ULONG genericArgumentsCount{ CorSigUncompressData(signature) };
            for (size_t i = 0; i < genericArgumentsCount; ++i) {
                PCCOR_SIGNATURE type{ consumeType(signature) };
                toString(metadata, type, result);

                if (i != genericArgumentsCount - 1) {
                    result += L", ";
                }
            }

            result += L'>';
            break;
        }

        case ELEMENT_TYPE_BYREF:
            result += L"ByRef ";
            toString(metadata, signature, result);
            break;

        default:
            ASSERT_NOT_REACHED();
        }
    }

    wstring Signature::toString(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
        wstring result;
        toString(metadata, signature, result);
        return result;
    }
}
}
