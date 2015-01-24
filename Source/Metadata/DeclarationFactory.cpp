#include "Metadata-Prefix.h"
#include "DeclarationFactory.h"

namespace NativeScript {
namespace Metadata {
    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    unique_ptr<DelegateDeclaration> DeclarationFactory::makeDelegateDeclaration(IMetaDataImport2* metadata, mdToken token) {
        switch (TypeFromToken(token)) {
        case mdtTypeDef: {
            return make_unique<DelegateDeclaration>(metadata, token);
        }

        case mdtTypeRef: {
            ComPtr<IMetaDataImport2> externalMetadata;
            mdTypeDef externalDelegateToken{ mdTokenNil };

            bool isResolved{ resolveTypeRef(metadata, token, externalMetadata.GetAddressOf(), &externalDelegateToken) };
            ASSERT(isResolved);

            return make_unique<DelegateDeclaration>(externalMetadata.Get(), externalDelegateToken);
        }

        case mdtTypeSpec: {
            PCCOR_SIGNATURE signature{ nullptr };
            ULONG signatureSize{ 0 };
            ASSERT_SUCCESS(metadata->GetTypeSpecFromToken(token, &signature, &signatureSize));

            CorElementType type1{ CorSigUncompressElementType(signature) };
            ASSERT(type1 == ELEMENT_TYPE_GENERICINST);

            CorElementType type2{ CorSigUncompressElementType(signature) };
            ASSERT(type2 == ELEMENT_TYPE_CLASS);

            mdToken openGenericDelegateToken{ CorSigUncompressToken(signature) };
            switch (TypeFromToken(openGenericDelegateToken)) {
            case mdtTypeDef: {
                return make_unique<GenericDelegateInstanceDeclaration>(metadata, openGenericDelegateToken, metadata, token);
            }

            case mdtTypeRef: {
                ComPtr<IMetaDataImport2> externalMetadata;
                mdTypeDef externalDelegateToken{ mdTokenNil };

                bool isResolved{ resolveTypeRef(metadata, openGenericDelegateToken, externalMetadata.GetAddressOf(), &externalDelegateToken) };
                ASSERT(isResolved);

                return make_unique<GenericDelegateInstanceDeclaration>(externalMetadata.Get(), externalDelegateToken, metadata, token);
            }

            default:
                ASSERT_NOT_REACHED();
            }
        }

        default:
            ASSERT_NOT_REACHED();
        }
    }

    unique_ptr<InterfaceDeclaration> DeclarationFactory::makeInterfaceDeclaration(IMetaDataImport2* metadata, mdToken token) {
        switch (TypeFromToken(token)) {
        case mdtTypeDef: {
            return make_unique<InterfaceDeclaration>(metadata, token);
        }

        case mdtTypeRef: {
            ComPtr<IMetaDataImport2> externalMetadata;
            mdTypeDef externalInterfaceToken{ mdTokenNil };

            bool isResolved{ resolveTypeRef(metadata, token, externalMetadata.GetAddressOf(), &externalInterfaceToken) };
            ASSERT(isResolved);

            return make_unique<InterfaceDeclaration>(externalMetadata.Get(), externalInterfaceToken);
        }

        case mdtTypeSpec: {
            PCCOR_SIGNATURE signature{ nullptr };
            ULONG signatureSize{ 0 };
            ASSERT_SUCCESS(metadata->GetTypeSpecFromToken(token, &signature, &signatureSize));

            CorElementType type1{ CorSigUncompressElementType(signature) };
            ASSERT(type1 == ELEMENT_TYPE_GENERICINST);

            CorElementType type2{ CorSigUncompressElementType(signature) };
            ASSERT(type2 == ELEMENT_TYPE_CLASS);

            mdToken openGenericDelegateToken{ CorSigUncompressToken(signature) };
            switch (TypeFromToken(openGenericDelegateToken)) {
            case mdtTypeDef: {
                return make_unique<GenericInterfaceInstanceDeclaration>(metadata, openGenericDelegateToken, metadata, token);
            }

            case mdtTypeRef: {
                ComPtr<IMetaDataImport2> externalMetadata;
                mdTypeDef externalDelegateToken{ mdTokenNil };

                bool isResolved{ resolveTypeRef(metadata, openGenericDelegateToken, externalMetadata.GetAddressOf(), &externalDelegateToken) };
                ASSERT(isResolved);

                return make_unique<GenericInterfaceInstanceDeclaration>(externalMetadata.Get(), externalDelegateToken, metadata, token);
            }

            default:
                ASSERT_NOT_REACHED();
            }

            break;
        }

        default:
            ASSERT_NOT_REACHED();
        }
    }
}
}
