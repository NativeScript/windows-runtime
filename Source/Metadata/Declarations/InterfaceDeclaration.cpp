#include "Metadata-Prefix.h"
#include "InterfaceDeclaration.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;
    using namespace Microsoft::WRL;

    InterfaceDeclaration::InterfaceDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(DeclarationKind::Interface, metadata, token) {
    }

    InterfaceDeclaration::InterfaceDeclaration(DeclarationKind kind, IMetaDataImport2* metadata, mdTypeDef token)
        : Base(kind, metadata, token) {
    }

    IID InterfaceDeclaration::id() const {
        return getGuidAttributeValue(_metadata.Get(), _token);
    }

    GenericInterfaceInstanceDeclaration::GenericInterfaceInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, IMetaDataImport2* closedMetadata, mdTypeSpec closedToken)
        : Base(DeclarationKind::GenericInterfaceInstance, openMetadata, openToken)
        , _closedMetadata{ closedMetadata }
        , _closedToken{ _closedToken } {

        ASSERT(closedMetadata);
        ASSERT(TypeFromToken(closedToken) == mdtTypeSpec);
        ASSERT(closedToken != mdTypeSpecNil);
    }

    wstring GenericInterfaceInstanceDeclaration::fullName() const {
        // TODO
        return Base::fullName();
    }

    IID GenericInterfaceInstanceDeclaration::id() const {
        // TODO: RoGetParameterizedTypeInstanceIID

        NOT_IMPLEMENTED();
    }
}
}
