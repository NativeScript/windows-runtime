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
        : DelegateDeclaration(DeclarationKind::Delegate, metadata, token) {
    }

    DelegateDeclaration::DelegateDeclaration(DeclarationKind kind, IMetaDataImport2* metadata, mdTypeDef token)
        : Base(kind, metadata, token)
        , _invokeMethod{ metadata, getInvokeMethodToken(metadata, token) } {
    }

    CLSID DelegateDeclaration::id() const {
        return getGuidAttributeValue(_metadata.Get(), _token);
    }

    const MethodDeclaration& DelegateDeclaration::invokeMethod() const {
        return _invokeMethod;
    }

    GenericDelegateDeclaration::GenericDelegateDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
        : Base(DeclarationKind::GenericDelegate, metadata, token) {
    }

    size_t GenericDelegateDeclaration::numberOfGenericParameters() const {
        ULONG count{ 0 };

        HCORENUM enumerator{ nullptr };
        ASSERT_SUCCESS(_metadata->EnumGenericParams(&enumerator, _token, nullptr, 0, nullptr));
        ASSERT_SUCCESS(_metadata->CountEnum(enumerator, &count));
        _metadata->CloseEnum(enumerator);

        return count;
    }

    GenericDelegateInstanceDeclaration::GenericDelegateInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, IMetaDataImport2* closedMetadata, mdTypeSpec closedToken)
        : Base(DeclarationKind::GenericDelegateInstance, openMetadata, openToken)
        , _closedMetadata{ closedMetadata }
        , _closedToken{ closedToken } {

        ASSERT(closedMetadata);
        ASSERT(TypeFromToken(closedToken) == mdtTypeSpec);
        ASSERT(closedToken != mdTypeSpecNil);
    }

    wstring GenericDelegateInstanceDeclaration::fullName() const {
        PCCOR_SIGNATURE signature{ nullptr };
        ULONG signatureSize{ 0 };
        ASSERT_SUCCESS(_closedMetadata->GetTypeSpecFromToken(_closedToken, &signature, &signatureSize));
        return Signature::toString(_closedMetadata.Get(), signature);
    }

    CLSID GenericDelegateInstanceDeclaration::id() const {
        CLSID guid = GenericInstanceIdBuilder::generateId(*this);
        return guid;
    }
}
}
