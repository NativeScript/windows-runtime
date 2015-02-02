#include "Runtime-Prefix.h"

#include "COMUnknownType.h"
#include "../GlobalObject.h"
#include "COMInterop.h"

namespace NativeScript {
using namespace JSC;

const FFITypeClassInfo COMUnknownType::s_info = { "COMUnknownType", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMUnknownType), CREATE_FFI_METHOD_TABLE(COMUnknownType) };

JSValue COMUnknownType::marshalNativeToJS(const JSCell* cell, ExecState* execState, const void* buffer) {
    COMInterop* interop = jsCast<GlobalObject*>(execState->lexicalGlobalObject())->interop();

    return interop->wrap(*reinterpret_cast<IUnknown**>(const_cast<void*>(buffer)), nullptr);
}

void COMUnknownType::marshalJSToNative(const JSCell* cell, ExecState* execState, JSValue value, void* buffer) {
    COMInterop* interop = jsCast<GlobalObject*>(execState->lexicalGlobalObject())->interop();

    interop->wrap(value, IID_IUnknown, reinterpret_cast<IUnknown**>(buffer));
}

ffi_type* COMUnknownType::getFFITypeStruct(const JSCell* cell) {
    return &ffi_type_pointer;
}
}