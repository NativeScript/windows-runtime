#pragma once

#include <JavaScriptCore/JSGlobalObject.h>
#include <JavaScriptCore/JSGlobalObjectInspectorController.h>

namespace NativeScript {

typedef class COMInterop Interop;

class GlobalObject : public JSC::JSGlobalObject {
public:
    typedef JSC::JSGlobalObject Base;

    static const bool needsDestruction = false;

    static const JSC::GlobalObjectMethodTable globalObjectMethodTable;

    static GlobalObject* create(JSC::VM& vm, JSC::Structure* structure) {
        GlobalObject* object = new (NotNull, JSC::allocateCell<GlobalObject>(vm.heap)) GlobalObject(vm, structure);
        object->finishCreation(vm);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    DECLARE_INFO;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, nullptr, prototype, JSC::TypeInfo(JSC::GlobalObjectType, StructureFlags), info());
    }

    static void visitChildren(JSC::JSCell* cell, JSC::SlotVisitor& visitor);

    Interop* interop() const {
        return _interopInstance.get();
    }

private:
    GlobalObject(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure, &GlobalObject::globalObjectMethodTable) {
    }

    ~GlobalObject();

    void finishCreation(JSC::VM& vm);

    static void destroy(JSC::JSCell* cell);

    static void queueTaskToEventLoop(const JSC::JSGlobalObject* globalObject, WTF::PassRefPtr<JSC::Microtask> task);

    std::unique_ptr<Inspector::JSGlobalObjectInspectorController> _inspectorController;

    JSC::WriteBarrier<Interop> _interopInstance;
};
}
