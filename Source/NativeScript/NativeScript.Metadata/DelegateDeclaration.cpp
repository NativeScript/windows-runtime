#include "pch.h"
#include "DelegateDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

namespace {

const wchar_t* const INVOKE_METHOD_NAME_W{L"Invoke"};

mdMethodDef getInvokeMethodToken(IMetaDataImport2* metaData, mdTypeDef token) {
    mdMethodDef invokeMethodToken{mdTokenNil};
    ASSERT_SUCCESS(metaData->FindMethod(token, INVOKE_METHOD_NAME_W, nullptr, 0, &invokeMethodToken));
    return invokeMethodToken;
}

}

DelegateDeclaration::DelegateDeclaration(IMetaDataImport2* metadata, mdTypeDef token)
    : Base(metadata, token)
    , _invokeMethod{metadata, getInvokeMethodToken(metadata, token)} {

}

CLSID DelegateDeclaration::id() const {
    return getGuidAttributeValue(_metadata.Get(), _token);
}

IteratorRange<DelegateDeclaration::ParameterIterator> DelegateDeclaration::parameters() const {
    return _invokeMethod.parameters();
}

GenericDelegateInstanceDeclaration::GenericDelegateInstanceDeclaration(IMetaDataImport2* openMetadata, mdTypeDef openToken, IMetaDataImport2* closedMetadata, mdTypeSpec closedToken)
    : Base(openMetadata, openToken)
    , _closedMetadata{closedMetadata}
    , _closedToken{_closedToken} {

    ASSERT(closedMetadata);
    ASSERT(TypeFromToken(closedToken) == mdtTypeSpec);
    ASSERT(closedToken != mdTypeSpecNil);
}

CLSID GenericDelegateInstanceDeclaration::id() const {
    // TODO: RoGetParameterizedTypeInstanceIID

    NOT_IMPLEMENTED();
}

IteratorRange<DelegateDeclaration::ParameterIterator> GenericDelegateInstanceDeclaration::parameters() const {
    // TODO

    return Base::parameters();
}

}
}
