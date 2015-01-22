#pragma once

#include "FFIType.h"
#include <JavaScriptCore/JSObject.h>
#include <type_traits>

namespace NativeScript {
template <typename T>
class FFIBooleanType : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    static_assert(std::is_convertible<T, bool>::value, "Type needs to be compatible with c++ bool");
    static_assert(std::is_convertible<bool, T>::value, "Type needs to be compatible with c++ bool");

    DECLARE_FFI_INFO

public:
    static FFIBooleanType<T>* create(JSC::VM& vm, JSC::Structure* structure) {
        FFIBooleanType* object = new (NotNull, JSC::allocateCell<FFIBooleanType<T>>(vm.heap)) FFIBooleanType<T>(vm, structure);
        object->finishCreation(vm);
        return object;
    }

    static const unsigned StructureFlags = IsFFITypeFlag | Base::StructureFlags;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    FFIBooleanType(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static JSC::JSValue marshalNativeToJS(const JSC::JSCell*, JSC::ExecState* execState, const void* buffer) {
        return JSC::jsBoolean(*reinterpret_cast<const bool*>(buffer));
    }

    static void marshalJSToNative(const JSC::JSCell*, JSC::ExecState* execState, JSC::JSValue value, void* buffer) {
        *reinterpret_cast<T*>(buffer) = value.toBoolean(execState);
    }

    static const ffi_type* getFFITypeStruct(const JSCell*) {
        return &ffi_type_uint8;
    }
};

template <typename T>
const FFITypeClassInfo FFIBooleanType<T>::s_info = { "FFIBooleanType", &Base::s_info, nullptr, CREATE_METHOD_TABLE(FFIBooleanType), CREATE_FFI_METHOD_TABLE(FFIBooleanType) };
}