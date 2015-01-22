#pragma once

#include "FFIType.h"
#include <JavaScriptCore/JSObject.h>

namespace NativeScript {
class FFIVoidType : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    DECLARE_FFI_INFO

public:
    static FFIVoidType* create(JSC::VM& vm, JSC::Structure* structure) {
        FFIVoidType* object = new (NotNull, JSC::allocateCell<FFIVoidType>(vm.heap)) FFIVoidType(vm, structure);
        object->finishCreation(vm);
        return object;
    }

    static const unsigned StructureFlags = IsFFITypeFlag | Base::StructureFlags;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    FFIVoidType(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static JSC::JSValue marshalNativeToJS(const JSC::JSCell*, JSC::ExecState*, const void*);

    static void marshalJSToNative(const JSC::JSCell*, JSC::ExecState*, JSC::JSValue, void*);

    static const ffi_type* getFFITypeStruct(const JSCell*);
};
}