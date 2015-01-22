#pragma once

#include "COMObjectMap.h"
#include "../FFI/FFIInterop.h"

namespace NativeScript {
class COMInterop : public FFIInterop {
    typedef FFIInterop Base;

    DECLARE_INFO;

public:
    static COMInterop* create(JSC::VM& vm, JSC::Structure* structure) {
        COMInterop* object = new (NotNull, JSC::allocateCell<COMInterop>(vm.heap)) COMInterop(vm, structure);
        object->finishCreation(vm);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static const bool needsDestruction = false;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    JSC::JSValue wrap(IUnknown*);

    COMObjectMap& objectMap() {
        return _objectMap;
    }

    ~COMInterop() = default;

private:
    COMInterop(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static void destroy(JSC::JSCell*);

    COMObjectMap _objectMap;
};
}