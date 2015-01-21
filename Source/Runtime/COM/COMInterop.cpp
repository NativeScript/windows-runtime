#include "Runtime-Prefix.h"
#include "COMInterop.h"

namespace NativeScript {
using namespace JSC;

const ClassInfo COMInterop::s_info = { "COMInterop", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMInterop) };

JSValue COMInterop::wrap(IUnknown* unknown) {
    if (!unknown) {
        return jsNull();
    }

    Microsoft::WRL::ComPtr<IUnknown> definiteUnknown;
    unknown->QueryInterface(definiteUnknown.GetAddressOf());

    if (COMObjectWrapper* existingWrapper = _objectMap.get(definiteUnknown.Get())) {
        return existingWrapper;
    }

    Structure* wrapperStructure = nullptr;

    Microsoft::WRL::ComPtr<IInspectable> inspectable;
    if (SUCCEEDED(unknown->QueryInterface(inspectable.GetAddressOf()))) {
        Microsoft::WRL::Wrappers::HString className;
        inspectable->GetRuntimeClassName(className.GetAddressOf());
        // TODO: get wrapper structure for this class
    } else {
        // default to the empty, IUnknown structure
        wrapperStructure = COMObjectWrapper::createStructure(globalObject()->vm(), globalObject(), globalObject()->objectPrototype());
    }

    COMObjectWrapper* wrapper = COMObjectWrapper::create(globalObject()->vm(), wrapperStructure, definiteUnknown.Get());
    _objectMap.set(wrapper);
    return wrapper;
}

COMInterop::~COMInterop() {
}
}