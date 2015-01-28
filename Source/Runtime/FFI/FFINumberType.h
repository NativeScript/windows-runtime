#pragma once

#include "FFIType.h"
#include <JavaScriptCore/JSObject.h>
#include <type_traits>

namespace NativeScript {
template <typename T>
class FFINumberType : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    static_assert(std::is_arithmetic<T>::value, "Type needs to be an arithmetic type");

    DECLARE_FFI_INFO

public:
    static FFINumberType<T>* create(JSC::VM& vm, JSC::Structure* structure, ffi_type* ffiType) {
        ASSERT(ffiType);

        FFINumberType* object = new (NotNull, JSC::allocateCell<FFINumberType<T>>(vm.heap)) FFINumberType<T>(vm, structure, ffiType);
        object->finishCreation(vm);
        return object;
    }

    static const unsigned StructureFlags = IsFFITypeFlag | Base::StructureFlags;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    FFINumberType(JSC::VM& vm, JSC::Structure* structure, ffi_type* ffiType)
        : Base(vm, structure)
        , _ffiType(ffiType) {
    }

    static JSC::JSValue marshalNativeToJS(const JSC::JSCell*, JSC::ExecState* execState, const void* buffer) {
        return JSC::jsNumber(*static_cast<const T*>(buffer));
    }

    static void marshalJSToNative(const JSC::JSCell*, JSC::ExecState* execState, JSC::JSValue value, void* buffer) {
        *reinterpret_cast<T*>(buffer) = value.toNumber(execState);
    }

    static ffi_type* getFFITypeStruct(const JSCell* cell) {
        const FFINumberType<T>* type = jsCast<const FFINumberType<T>*>(cell);
        return type->_ffiType;
    }

    ffi_type* _ffiType;
};

template <typename T>
const FFITypeClassInfo FFINumberType<T>::s_info = { "FFINumberType", &Base::s_info, nullptr, CREATE_METHOD_TABLE(FFINumberType), CREATE_FFI_METHOD_TABLE(FFINumberType) };
}