#include "pch.h"
#include "DelegateDeclaration.h"

namespace NativeScript {
namespace Metadata {

using namespace std;

namespace {

const wchar_t* const INVOKE_METHOD_NAME_W{L"Invoke"};

mdMethodDef getInvokeMethodToken(IMetaDataImport2* metaData, mdTypeDef token) {
    mdMethodDef invokeMethodToken{mdMethodDefNil};
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

size_t DelegateDeclaration::numberOfParameters() const {
    return _invokeMethod.numberOfParameters();
}

}
}
