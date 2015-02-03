#include "Metadata-Prefix.h"
#include "Signature.h"
#include "TypeCache.h"

namespace NativeScript {
namespace Metadata {
    using namespace std;

    namespace {
        void stringify(IMetaDataImport2* metadata, PCCOR_SIGNATURE& signature, wstring& result) {
            CorElementType elementType{ CorSigUncompressElementType(signature) };
            switch (elementType) {
            case ELEMENT_TYPE_VOID:
                TypeCache::instance().voidType().toStringInternal(result);
                break;

            case ELEMENT_TYPE_BOOLEAN:
                TypeCache::instance().booleanType().toStringInternal(result);
                break;

            case ELEMENT_TYPE_CHAR:
                TypeCache::instance().char16Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_I1:
                TypeCache::instance().int8Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_U1:
                TypeCache::instance().uInt8Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_I2:
                TypeCache::instance().int16Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_U2:
                TypeCache::instance().uInt16Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_I4:
                TypeCache::instance().int32Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_U4:
                TypeCache::instance().uInt32Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_I8:
                TypeCache::instance().int64Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_U8:
                TypeCache::instance().uInt64Type().toStringInternal(result);
                break;

            case ELEMENT_TYPE_R4:
                TypeCache::instance().singleType().toStringInternal(result);
                break;

            case ELEMENT_TYPE_R8:
                TypeCache::instance().doubleType().toStringInternal(result);
                break;

            case ELEMENT_TYPE_STRING:
                TypeCache::instance().stringType().toStringInternal(result);
                break;

            case ELEMENT_TYPE_VALUETYPE: {
                mdToken token{ CorSigUncompressToken(signature) };
                wstring className{ getTypeName(metadata, token) };
                if (className == L"System.Guid") {
                    TypeCache::instance().guidType().toStringInternal(result);
                } else {
                    result.append(className);
                }
                break;
            }

            case ELEMENT_TYPE_CLASS: {
                mdToken token{ CorSigUncompressToken(signature) };
                result.append(getTypeName(metadata, token));
                break;
            }

            case ELEMENT_TYPE_OBJECT:
                TypeCache::instance().objectType().toStringInternal(result);
                break;

            case ELEMENT_TYPE_SZARRAY:
                stringify(metadata, signature, result);
                result.append(L"[]");
                break;

            case ELEMENT_TYPE_VAR: {
                ULONG index{ CorSigUncompressData(signature) };
                result.append(L"Var!");
                result.append(to_wstring(index));
                break;
            }

            case ELEMENT_TYPE_GENERICINST: {
                CorElementType genericType{ CorSigUncompressElementType(signature) };
                ASSERT(genericType == ELEMENT_TYPE_CLASS);

                mdToken token{ CorSigUncompressToken(signature) };
                result.append(getTypeName(metadata, token));

                result.append(L"<");

                ULONG genericArgumentsCount{ CorSigUncompressData(signature) };
                for (size_t i = 0; i < genericArgumentsCount; ++i) {
                    stringify(metadata, signature, result);

                    if (i != genericArgumentsCount - 1) {
                        result.append(L", ");
                    }
                }

                result.append(L">");
                break;
            }

            case ELEMENT_TYPE_BYREF:
                result.append(L"ByRef ");
                stringify(metadata, signature, result);
                break;

            default:
                ASSERT_NOT_REACHED();
                }
            }
        }

        Signature::Signature(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature)
            : _metadata(metadata)
            , _signature(signature) {
        }

        wstring Signature::toString() {
            wstring result;
            PCCOR_SIGNATURE copy{ _signature };
            stringify(_metadata, copy, result);
            return result;
        }
    }
}
