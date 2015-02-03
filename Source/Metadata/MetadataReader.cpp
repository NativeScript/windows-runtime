#include "Metadata-Prefix.h"
#include "MetadataReader.h"
#include "TypeCache.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    // TODO: Create HCorenum wrapper

    // TODO: Use decl_iterator, specific_decl_iterator, filtered_decl_iterator

    const Type* MetadataReader::findByName(const wchar_t* fullName) {
        HStringReference fullNameRef{ fullName };
        return findByName(fullNameRef.Get());
        }

        const Type* MetadataReader::findByName(HSTRING fullName) {
            // Look in cache

            if (WindowsGetStringLen(fullName) == 0) {
                return &TypeCache::instance().getOrCreateNamespace(WindowsGetStringRawBuffer(fullName, nullptr));
            }

            ComPtr<IMetaDataImport2> metadata;
            mdTypeDef token{ mdTokenNil };

            HRESULT getMetadataFileResult{ RoGetMetaDataFile(fullName, nullptr, nullptr, metadata.GetAddressOf(), &token) };

            if (FAILED(getMetadataFileResult)) {
                if (getMetadataFileResult == RO_E_METADATA_NAME_IS_NAMESPACE) {
                    return &TypeCache::instance().getOrCreateNamespace(WindowsGetStringRawBuffer(fullName, nullptr));
                }

                // Find indexOf '<'

                return nullptr;
            }

            return &TypeCache::instance().getOrCreateTypeDeclaration(metadata.Get(), token);
        }

        const Type& MetadataReader::parseType(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
            CorElementType elementType{ CorSigUncompressElementType(signature) };
            switch (elementType) {
            case ELEMENT_TYPE_VOID:
                return TypeCache::instance().voidType();

            case ELEMENT_TYPE_BOOLEAN:
                return TypeCache::instance().booleanType();

            case ELEMENT_TYPE_CHAR:
                return TypeCache::instance().char16Type();

            case ELEMENT_TYPE_I1:
                return TypeCache::instance().int8Type();

            case ELEMENT_TYPE_U1:
                return TypeCache::instance().uInt8Type();

            case ELEMENT_TYPE_I2:
                return TypeCache::instance().int16Type();

            case ELEMENT_TYPE_U2:
                return TypeCache::instance().uInt16Type();

            case ELEMENT_TYPE_I4:
                return TypeCache::instance().int32Type();

            case ELEMENT_TYPE_U4:
                return TypeCache::instance().uInt32Type();

            case ELEMENT_TYPE_I8:
                return TypeCache::instance().int64Type();

            case ELEMENT_TYPE_U8:
                return TypeCache::instance().uInt64Type();

            case ELEMENT_TYPE_R4:
                return TypeCache::instance().singleType();

            case ELEMENT_TYPE_R8:
                return TypeCache::instance().doubleType();

            case ELEMENT_TYPE_STRING:
                return TypeCache::instance().stringType();

            case ELEMENT_TYPE_VALUETYPE: {
            case ELEMENT_TYPE_CLASS: {
                mdToken token{ CorSigUncompressToken(signature) };
                return TypeCache::instance().getOrCreateTypeDeclaration(metadata, token);
            }

            case ELEMENT_TYPE_OBJECT:
                return TypeCache::instance().objectType();

            case ELEMENT_TYPE_SZARRAY:
                return TypeCache::instance().getOrCreateArrayType(metadata, signature);

            case ELEMENT_TYPE_VAR:
                return TypeCache::instance().getOrCreateGenericParameterType(metadata, signature);

            case ELEMENT_TYPE_GENERICINST: {
                return TypeCache::instance().getOrCreateGenericInstanceType(metadata, signature);
            }

            case ELEMENT_TYPE_BYREF:
                return TypeCache::instance().getOrCreateReferenceType(metadata, signature);

            default:
                ASSERT_NOT_REACHED();
            }
            }
        }
    }
}
