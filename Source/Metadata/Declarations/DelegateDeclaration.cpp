#include "Metadata-Prefix.h"
#include "DelegateDeclaration.h"
#include "Signature.h"
#include "GenericInstanceIdBuilder.h"

namespace NativeScript {
namespace Metadata {

    using namespace std;

    namespace {

        const wchar_t* const INVOKE_METHOD_NAME_W{ L"Invoke" };

        mdMethodDef getInvokeMethodToken(IMetaDataImport2* metaData, mdTypeDef token) {
            mdMethodDef invokeMethodToken{ mdTokenNil };
            ASSERT_SUCCESS(metaData->FindMethod(token, INVOKE_METHOD_NAME_W, nullptr, 0, &invokeMethodToken));
            return invokeMethodToken;
        }
    }

    DelegateDeclaration::DelegateDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : DelegateDeclaration(ElementType::Delegate, metadata, token) {
    }

    DelegateDeclaration::DelegateDeclaration(ElementType elementType, IMetaDataImport2* metadata, mdTypeDef token)
        : Base(elementType, metadata, token)
        , _invokeMethod{ metadata, getInvokeMethodToken(metadata, token) } {
    }

    CLSID DelegateDeclaration::id() const {
        return getGuidAttributeValue(_metadata.Get(), _token);
    }

    const MethodDeclaration& DelegateDeclaration::invokeMethod() const {
        return _invokeMethod;
    }

    GenericDelegateDeclaration::GenericDelegateDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(ElementType::GenericDelegate, metadata, token) {
    }

    size_t GenericDelegateDeclaration::numberOfGenericParameters() const {
        ULONG count{ 0 };

        HCORENUM enumerator{ nullptr };
        ASSERT_SUCCESS(_metadata->EnumGenericParams(&enumerator, _token, nullptr, 0, nullptr));
        ASSERT_SUCCESS(_metadata->CountEnum(enumerator, &count));
        _metadata->CloseEnum(enumerator);

        return count;
    }

    GenericDelegateInstanceDeclaration::GenericDelegateInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, IMetaDataImport2* closedMetadata, PCCOR_SIGNATURE signature)
        : Base(ElementType::GenericDelegateInstance, openMetadata, openToken)
        , _closedMetadata{ closedMetadata }
        , _signature{ signature } {

        ASSERT(closedMetadata);
        ASSERT(signature);
    }

    wstring GenericDelegateInstanceDeclaration::fullName() const {
        return Signature(_closedMetadata.Get(), _signature).toString();
    }

    CLSID GenericDelegateInstanceDeclaration::id() const {
        CLSID guid = GenericInstanceIdBuilder::generateId(*this);
        return guid;
    }
}
}
