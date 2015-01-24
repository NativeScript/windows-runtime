#include "Metadata-Prefix.h"
#include "Signature.h"
#include <sstream>

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
            // TODO: CustomMod
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

    wstring Signature::toString(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
        CorElementType elementType{ CorSigUncompressElementType(signature) };
        switch (elementType) {
        case ELEMENT_TYPE_VOID:
            return L"Void";

        case ELEMENT_TYPE_BOOLEAN:
            return L"Boolean";

        case ELEMENT_TYPE_CHAR:
            return L"Char16";

        case ELEMENT_TYPE_I1:
            return L"Int8";

        case ELEMENT_TYPE_U1:
            return L"UInt8";

        case ELEMENT_TYPE_I2:
            return L"Int16";

        case ELEMENT_TYPE_U2:
            return L"UInt16";

        case ELEMENT_TYPE_I4:
            return L"Int32";

        case ELEMENT_TYPE_U4:
            return L"UInt32";

        case ELEMENT_TYPE_I8:
            return L"Int64";

        case ELEMENT_TYPE_U8:
            return L"UInt64";

        case ELEMENT_TYPE_R4:
            return L"Single";

        case ELEMENT_TYPE_R8:
            return L"Double";

        case ELEMENT_TYPE_STRING:
            return L"String";

        case ELEMENT_TYPE_VALUETYPE: {
            mdToken token{ CorSigUncompressToken(signature) };
            identifier className = getClassName(metadata, token);
            if (wcscmp(className.data(), L"System.Guid") == 0) {
                return L"Guid";
            } else {
                return className.data();
            }
        }

        case ELEMENT_TYPE_CLASS: {
            mdToken token{ CorSigUncompressToken(signature) };
            return getClassName(metadata, token).data();
        }

        case ELEMENT_TYPE_OBJECT:
            return L"Object";

        case ELEMENT_TYPE_SZARRAY:
            // TODO: CustomMod
            return toString(metadata, signature) + L"[]";

        case ELEMENT_TYPE_VAR: {
            ULONG index{ CorSigUncompressData(signature) };
            return L"Var!" + to_wstring(index);
        }

        case ELEMENT_TYPE_GENERICINST: {
            wostringstream result;

            CorElementType genericType{ CorSigUncompressElementType(signature) };
            ASSERT(genericType == ELEMENT_TYPE_CLASS);

            mdToken token{ CorSigUncompressToken(signature) };
            result << getClassName(metadata, token).data();

            result << L'<';

            ULONG genericArgumentsCount{ CorSigUncompressData(signature) };
            for (size_t i = 0; i < genericArgumentsCount; ++i) {
                PCCOR_SIGNATURE type{ consumeType(signature) };
                result << toString(metadata, type);

                if (i != genericArgumentsCount - 1) {
                    result << L", ";
                }
            }

            result << L'>';

            wstring str{ result.str() };
            return str;
        }

        case ELEMENT_TYPE_BYREF:
            consumeType(signature);
            return L"ref " + toString(metadata, signature);

        default:
            ASSERT_NOT_REACHED();
        }
    }
}
}
