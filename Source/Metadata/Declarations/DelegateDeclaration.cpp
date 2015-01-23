#include "Metadata-Prefix.h"
#include "DelegateDeclaration.h"

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

    PCCOR_SIGNATURE DelegateDeclaration::returnType() const {
        return _invokeMethod.returnType();
    }

    IteratorRange<DelegateDeclaration::ParameterIterator> DelegateDeclaration::parameters() const {
        return _invokeMethod.parameters();
    }

    GenericDelegateInstanceDeclaration::GenericDelegateInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, IMetaDataImport2* closedMetadata, mdTypeSpec closedToken)
        : Base(DeclarationKind::GenericDelegateInstance, openMetadata, openToken)
        , _closedMetadata{ closedMetadata }
        , _closedToken{ _closedToken } {

        ASSERT(closedMetadata);
        ASSERT(TypeFromToken(closedToken) == mdtTypeSpec);
        ASSERT(closedToken != mdTypeSpecNil);
    }

    wstring GenericDelegateInstanceDeclaration::fullName() const {
        // TODO
        return Base::fullName();
    }

    CLSID GenericDelegateInstanceDeclaration::id() const {
        // TODO: RoGetParameterizedTypeInstanceIID

        NOT_IMPLEMENTED();
    }

    PCCOR_SIGNATURE GenericDelegateInstanceDeclaration::returnType() const {
        // TODO

        return Base::returnType();
    }

    IteratorRange<DelegateDeclaration::ParameterIterator> GenericDelegateInstanceDeclaration::parameters() const {
        // TODO

        return Base::parameters();
    }
}
}
