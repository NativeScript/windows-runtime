#include "Metadata-Prefix.h"
#include "InterfaceDeclaration.h"
#include "Signature.h"
#include "GenericInstanceIdBuilder.h"

namespace NativeScript {
namespace Metadata {
    using namespace std;
    using namespace Microsoft::WRL::Wrappers;
    using namespace Microsoft::WRL;

    InterfaceDeclaration::InterfaceDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : InterfaceDeclaration(DeclarationKind::Interface, metadata, token) {
    }

    InterfaceDeclaration::InterfaceDeclaration(DeclarationKind kind, IMetaDataImport2* metadata, mdTypeDef token)
        : Base(kind, metadata, token) {
    }

    IID InterfaceDeclaration::id() const {
        return getGuidAttributeValue(_metadata.Get(), _token);
    }

    GenericInterfaceDeclaration::GenericInterfaceDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(DeclarationKind::GenericInterface, metadata, token) {
    }

    size_t GenericInterfaceDeclaration::numberOfGenericParameters() const {
        ULONG count{ 0 };

        HCORENUM enumerator{ nullptr };
        ASSERT_SUCCESS(_metadata->EnumGenericParams(&enumerator, _token, nullptr, 0, nullptr));
        ASSERT_SUCCESS(_metadata->CountEnum(enumerator, &count));
        _metadata->CloseEnum(enumerator);

        return count;
    }

    GenericInterfaceInstanceDeclaration::GenericInterfaceInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, IMetaDataImport2* closedMetadata, mdTypeSpec closedToken)
        : Base(DeclarationKind::GenericInterfaceInstance, openMetadata, openToken)
        , _closedMetadata{ closedMetadata }
        , _closedToken{ closedToken } {
        ASSERT(closedMetadata);
        ASSERT(TypeFromToken(closedToken) == mdtTypeSpec);
        ASSERT(closedToken != mdTypeSpecNil);
    }

    wstring GenericInterfaceInstanceDeclaration::fullName() const {
        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };
        ASSERT_SUCCESS(_closedMetadata->GetTypeSpecFromToken(_closedToken, &signature, &signatureSize));
        return Signature::toString(_closedMetadata.Get(), signature);
    }

    IID GenericInterfaceInstanceDeclaration::id() const {
        IID guid = GenericInstanceIdBuilder::generateId(*this);
        return guid;
    }
}
}
