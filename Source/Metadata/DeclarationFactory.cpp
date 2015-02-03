#include "Metadata-Prefix.h"
#include "DeclarationFactory.h"

namespace NativeScript {
namespace Metadata {
    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    unique_ptr<TypeDeclaration> DeclarationFactory::makeTypeDefDeclaration(IMetaDataImport2* metadata, mdTypeDef token, const wchar_t* fullName) {
        ASSERT(metadata);
        ASSERT(TypeFromToken(token) == mdtTypeDef);
        ASSERT(token != mdTypeDefNil);

        DWORD flags{ 0 };
        mdToken parentToken{ mdTokenNil };
        ASSERT_SUCCESS(metadata->GetTypeDefProps(token, nullptr, 0, nullptr, &flags, &parentToken));

        if (IsTdClass(flags)) {
            wstring parentName { getBase }

            if (wcscmp(parentName.data(), SYSTEM_ENUM_W) == 0) {
                return make_unique<EnumDeclaration>(metadata, token);
            }

            if (wcscmp(parentName.data(), SYSTEM_VALUETYPE_W) == 0) {
                return make_unique<StructDeclaration>(metadata, token);
            }

            if (wcscmp(parentName.data(), SYSTEM_MULTICASTDELEGATE_W) == 0) {
                if (wcsstr(fullName, L"`")) {
                    return make_unique<GenericDelegateDeclaration>(metadata, token);
                } else {
                    return make_unique<DelegateDeclaration>(metadata, token);
                }
            }

            return make_unique<ClassDeclaration>(metadata, token);
        }

        if (IsTdInterface(flags)) {
            if (wcsstr(fullName, L"`")) {
                return make_unique<GenericInterfaceDeclaration>(metadata, token);
            } else {
                return make_unique<InterfaceDeclaration>(metadata, token);
            }
        }

        ASSERT_NOT_REACHED();
    }

    unique_ptr<NamespaceDeclaration> DeclarationFactory::makeNamespaceDeclaration(const wchar_t* fullName) {
        return make_unique<NamespaceDeclaration>(*fullName == L'\0' ? L"" : fullName);
    }

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

            PCCOR_SIGNATURE closedSignature{ getTypeSpecSignature(metadata, token) };

            mdToken openGenericDelegateToken{ CorSigUncompressToken(signature) };
            switch (TypeFromToken(openGenericDelegateToken)) {
            case mdtTypeDef: {
                return make_unique<GenericDelegateInstanceDeclaration>(metadata, openGenericDelegateToken, metadata, closedSignature);
            }

            case mdtTypeRef: {
                ComPtr<IMetaDataImport2> externalMetadata;
                mdTypeDef externalDelegateToken{ mdTokenNil };

                bool isResolved{ resolveTypeRef(metadata, openGenericDelegateToken, externalMetadata.GetAddressOf(), &externalDelegateToken) };
                ASSERT(isResolved);

                return make_unique<GenericDelegateInstanceDeclaration>(externalMetadata.Get(), externalDelegateToken, metadata, closedSignature);
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

            PCCOR_SIGNATURE closedSignature{ getTypeSpecSignature(metadata, token) };

            mdToken openGenericDelegateToken{ CorSigUncompressToken(signature) };
            switch (TypeFromToken(openGenericDelegateToken)) {
            case mdtTypeDef: {
                return make_unique<GenericInterfaceInstanceDeclaration>(metadata, openGenericDelegateToken, metadata, closedSignature);
            }

            case mdtTypeRef: {
                ComPtr<IMetaDataImport2> externalMetadata;
                mdTypeDef externalDelegateToken{ mdTokenNil };

                bool isResolved{ resolveTypeRef(metadata, openGenericDelegateToken, externalMetadata.GetAddressOf(), &externalDelegateToken) };
                ASSERT(isResolved);

                return make_unique<GenericInterfaceInstanceDeclaration>(externalMetadata.Get(), externalDelegateToken, metadata, closedSignature);
            }

            default:
                ASSERT_NOT_REACHED();
            }
        }

        default:
            ASSERT_NOT_REACHED();
        }
    }
}
}
