#include "Runtime-Prefix.h"

#include "HSTRINGType.h"

namespace NativeScript {
using namespace JSC;

class MovableHString {
public:
    MovableHString(Microsoft::WRL::Wrappers::HString&& other) {
        _ptr = other.Detach();
    }

    MovableHString(MovableHString&& other) {
        _ptr = other._ptr;
        other._ptr = nullptr;
    }

    ~MovableHString() {
        WindowsDeleteString(_ptr);
    }

private:
    HSTRING _ptr;
};

const FFITypeClassInfo HSTRINGType::s_info = { "HSTRINGType", &Base::s_info, nullptr, CREATE_METHOD_TABLE(HSTRINGType), CREATE_FFI_METHOD_TABLE(HSTRINGType) };

JSValue HSTRINGType::marshalNativeToJS(const JSCell* cell, ExecState* execState, const void* buffer) {
    return jsString(execState, *reinterpret_cast<HSTRING*>(const_cast<void*>(buffer)));
}

void HSTRINGType::marshalJSToNative(const JSCell* cell, ExecState* execState, JSValue value, void* buffer) {
    WTF::String stringValue;
    if (value.isUndefinedOrNull()) {
        stringValue = emptyString();
    } else {
        stringValue = value.toWTFString(execState);
    }

    Microsoft::WRL::Wrappers::HString nativeString(stringValue.createHString());
    *reinterpret_cast<HSTRING*>(buffer) = nativeString.Get();
    ReleasePool<MovableHString>::releaseSoon(std::move(nativeString));
}

ffi_type* HSTRINGType::getFFITypeStruct(const JSCell* cell) {
    return &ffi_type_pointer;
}
}