#include "Runtime-prefix.h"
#include "COMObjectWrapper.h"
#include "../GlobalObject.h"
#include "COMInterop.h"

namespace NativeScript {
using namespace JSC;

const ClassInfo COMObjectWrapper::s_info = { "COMObjectWrapper", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMObjectWrapper) };

WTF::String COMObjectWrapper::className(const JSObject* object) {
    const COMObjectWrapper* wrapper = jsCast<const COMObjectWrapper*>(object);
    Microsoft::WRL::ComPtr<IInspectable> inspectable;
    if (SUCCEEDED(wrapper->_wrappedObject.As(&inspectable))) {
        Microsoft::WRL::Wrappers::HString className;
        inspectable->GetRuntimeClassName(className.GetAddressOf());
        return WTF::String(className.Get());
    }

    return Base::className(object);
}

COMObjectWrapper::~COMObjectWrapper() {
    GlobalObject* globalObject = jsCast<GlobalObject*>(this->globalObject());
    globalObject->interop()->objectMap().remove(this);

    // TODO: use DelayedReleaseScope
}

void COMObjectWrapper::finishCreation(VM& vm, IUnknown* wrappedObject) {
    Base::finishCreation(vm);
    ASSERT(wrappedObject);

#if !defined(NDEBUG)
    Microsoft::WRL::ComPtr<IUnknown> definiteUnknown;
    wrappedObject->QueryInterface(definiteUnknown.GetAddressOf());
    ASSERT(definiteUnknown.Get() == wrappedObject);
#endif

    _wrappedObject = wrappedObject;
}
}