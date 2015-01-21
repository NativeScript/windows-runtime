#pragma once

#include "Runtime-Prefix.h"
#include <JavaScriptCore/JSGlobalObject.h>

namespace Inspector {
class JSGlobalObjectInspectorController;
}

namespace NativeScript {

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

private:
    GlobalObject(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure, &GlobalObject::globalObjectMethodTable) {
    }

    ~GlobalObject();

    void finishCreation(JSC::VM& vm);

    static void destroy(JSC::JSCell* cell);

    static void queueTaskToEventLoop(const JSC::JSGlobalObject* globalObject, WTF::PassRefPtr<JSC::Microtask> task);

    std::unique_ptr<Inspector::JSGlobalObjectInspectorController> _inspectorController;
};
}
