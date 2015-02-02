#pragma once

#include <JavaScriptCore/JSObject.h>
#include "../FFI/FFIType.h"

namespace NativeScript {
class HSTRINGType : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    DECLARE_FFI_INFO

public:
    static HSTRINGType* create(JSC::VM& vm, JSC::Structure* structure) {
        HSTRINGType* object = new (NotNull, JSC::allocateCell<HSTRINGType>(vm.heap)) HSTRINGType(vm, structure);
        object->finishCreation(vm);
        return object;
    }

    static const unsigned StructureFlags = IsFFITypeFlag | Base::StructureFlags;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    HSTRINGType(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static JSC::JSValue marshalNativeToJS(const JSC::JSCell*, JSC::ExecState* execState, const void* buffer);

    static void marshalJSToNative(const JSC::JSCell*, JSC::ExecState* execState, JSC::JSValue value, void* buffer);

    static ffi_type* getFFITypeStruct(const JSCell*);
};
}