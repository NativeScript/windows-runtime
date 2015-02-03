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
        : InterfaceDeclaration(ElementType::Interface, metadata, token) {
    }

    InterfaceDeclaration::InterfaceDeclaration(ElementType elementType, IMetaDataImport2* metadata, mdTypeDef token)
        : Base(elementType, metadata, token) {
    }

    IID InterfaceDeclaration::id() const {
        return getGuidAttributeValue(_metadata.Get(), _token);
    }

    GenericInterfaceDeclaration::GenericInterfaceDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(ElementType::GenericInterface, metadata, token) {
    }

    size_t GenericInterfaceDeclaration::numberOfGenericParameters() const {
        ULONG count{ 0 };

        HCORENUM enumerator{ nullptr };
        ASSERT_SUCCESS(_metadata->EnumGenericParams(&enumerator, _token, nullptr, 0, nullptr));
        ASSERT_SUCCESS(_metadata->CountEnum(enumerator, &count));
        _metadata->CloseEnum(enumerator);

        return count;
    }

    GenericInterfaceInstanceDeclaration::GenericInterfaceInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, vector<Type&> genericArguments)
        : Base(ElementType::GenericInterfaceInstance, openMetadata, openToken)
        , _genericArguments{ genericArguments } {
        ASSERT(genericArguments.size() > 0);
    }

    wstring GenericInterfaceInstanceDeclaration::fullName() const {
        return;
    }

    IID GenericInterfaceInstanceDeclaration::id() const {
        IID guid = GenericInstanceIdBuilder::generateId(*this);
        return guid;
    }
}
}
