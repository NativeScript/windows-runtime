#include "Runtime-Prefix.h"
#include "FFIVoidType.h"

namespace NativeScript {
using namespace JSC;

const FFITypeClassInfo FFIVoidType::s_info = { "FFIVoidType", &Base::s_info, nullptr, CREATE_METHOD_TABLE(FFIVoidType), CREATE_FFI_METHOD_TABLE(FFIVoidType) };

JSValue FFIVoidType::marshalNativeToJS(const JSCell*, ExecState*, const void*) {
    return jsUndefined();
}

void FFIVoidType::marshalJSToNative(const JSCell*, ExecState*, JSValue, void*) {
}

const ffi_type* FFIVoidType::getFFITypeStruct(const JSCell*) {
    return &ffi_type_void;
}
}