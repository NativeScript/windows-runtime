#pragma once

#include <JavaScriptCore/JSObject.h>
#include "../FFI/FFIType.h"

namespace NativeScript {
class COMUnknownType : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    DECLARE_FFI_INFO

public:
    static COMUnknownType* create(JSC::VM& vm, JSC::Structure* structure) {
        COMUnknownType* object = new (NotNull, JSC::allocateCell<COMUnknownType>(vm.heap)) COMUnknownType(vm, structure);
        object->finishCreation(vm);
        return object;
    }

    static const unsigned StructureFlags = IsFFITypeFlag | Base::StructureFlags;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    COMUnknownType(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static JSC::JSValue marshalNativeToJS(const JSC::JSCell*, JSC::ExecState* execState, const void* buffer);

    static void marshalJSToNative(const JSC::JSCell*, JSC::ExecState* execState, JSC::JSValue value, void* buffer);

    static ffi_type* getFFITypeStruct(const JSCell*);
};
}