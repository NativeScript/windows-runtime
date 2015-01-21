#include "Runtime-Prefix.h"

#include "GlobalObject.h"
#include <JavaScriptCore/JSGlobalObjectInspectorController.h>
#include <JavaScriptCore/Microtask.h>
#include <WTF/RunLoop.h>

#include "COM/COMInterop.h"

namespace NativeScript {
using namespace JSC;

const ClassInfo GlobalObject::s_info = { "NativeScriptGlobal", &Base::s_info, nullptr, CREATE_METHOD_TABLE(GlobalObject) };

const GlobalObjectMethodTable GlobalObject::globalObjectMethodTable = { &allowsAccessFrom, &supportsProfiling, &supportsRichSourceInfo, &shouldInterruptScript, &javaScriptExperimentsEnabled, &queueTaskToEventLoop, &shouldInterruptScriptBeforeTimeout };

GlobalObject::~GlobalObject() {
    this->_inspectorController->globalObjectDestroyed();
}

extern "C" void JSSynchronousGarbageCollectForDebugging(ExecState*);
static EncodedJSValue JSC_HOST_CALL collectGarbage(ExecState* execState) {
    JSSynchronousGarbageCollectForDebugging(execState->lexicalGlobalObject()->globalExec());
    return JSValue::encode(jsUndefined());
}

void GlobalObject::finishCreation(VM& vm) {
    Base::finishCreation(vm);

    ExecState* globalExec = this->globalExec();

    this->_inspectorController = std::make_unique<Inspector::JSGlobalObjectInspectorController>(*this);
    this->setConsoleClient(this->_inspectorController->consoleClient());
    this->putDirect(vm, vm.propertyNames->global, this, DontEnum | ReadOnly | DontDelete);

    this->putDirectNativeFunction(vm, this, Identifier(globalExec, WTF::ASCIILiteral("__collect")), 0, &collectGarbage, NoIntrinsic, DontEnum | Attribute::Function);

    _interopInstance.set(vm, this, COMInterop::create(vm, COMInterop::createStructure(vm, this, jsNull())));
}

void GlobalObject::visitChildren(JSCell* cell, SlotVisitor& visitor) {
    GlobalObject* globalObject = jsCast<GlobalObject*>(cell);
    Base::visitChildren(globalObject, visitor);

    visitor.append(&globalObject->_interopInstance);
}

void GlobalObject::destroy(JSCell* cell) {
    static_cast<GlobalObject*>(cell)->~GlobalObject();
}

void GlobalObject::queueTaskToEventLoop(const JSGlobalObject* globalObject, WTF::PassRefPtr<Microtask> task) {
    WTF::RunLoop::current().dispatch([globalObject, task] {
        task->run(const_cast<JSGlobalObject*>(globalObject)->globalExec());
    });
}
}
