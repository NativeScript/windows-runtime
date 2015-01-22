#pragma once

#include <JavaScriptCore/JSObject.h>
#include "FFIVoidType.h"
#include "FFINumberType.h"
#include "FFIBooleanType.h"

namespace NativeScript {

class FFIInterop : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    DECLARE_INFO;

public:
    static FFIInterop* create(JSC::VM& vm, JSC::Structure* structure) {
        FFIInterop* object = new (NotNull, JSC::allocateCell<FFIInterop>(vm.heap)) FFIInterop(vm, structure);
        object->finishCreation(vm);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static const bool needsDestruction = false;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    ~FFIInterop() = default;

protected:
    FFIInterop(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    void finishCreation(JSC::VM&);

    static void visitChildren(JSC::JSCell*, JSC::SlotVisitor&);

private:
    static void destroy(JSC::JSCell*);

    JSC::WriteBarrier<FFIVoidType> _voidType;
    JSC::WriteBarrier<FFINumberType<int8_t>> _int8Type;
    JSC::WriteBarrier<FFINumberType<uint8_t>> _uint8Type;
    JSC::WriteBarrier<FFINumberType<int16_t>> _int16Type;
    JSC::WriteBarrier<FFINumberType<uint16_t>> _uint16Type;
    JSC::WriteBarrier<FFINumberType<int32_t>> _int32Type;
    JSC::WriteBarrier<FFINumberType<uint32_t>> _uint32Type;
    JSC::WriteBarrier<FFINumberType<int64_t>> _int64Type;
    JSC::WriteBarrier<FFINumberType<uint64_t>> _uint64Type;
    JSC::WriteBarrier<FFINumberType<float>> _floatType;
    JSC::WriteBarrier<FFINumberType<double>> _doubleType;
    JSC::WriteBarrier<FFIBooleanType<bool>> _boolType;
};
}