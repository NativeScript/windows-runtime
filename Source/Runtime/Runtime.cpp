#include "pch.h"
#include <JavaScriptCore/InitializeThreading.h>
#include <JavaScriptCore/APICast.h>
#include <JavaScriptCore/FunctionConstructor.h>
#include <JavaScriptCore/JSGlobalObjectInspectorController.h>
#include <JavaScriptCore/StrongInlines.h>
#include "GlobalObject.h"

#include "Runtime.h"

using namespace JSC;

namespace NativeScript {
    struct RuntimeImpl {
        WTF::RefPtr<VM> vm;
        Strong<GlobalObject> globalObject;

        RuntimeImpl() {
            this->vm = VM::create(SmallHeap);

            JSLockHolder lock(*this->vm);
            this->globalObject = Strong<GlobalObject>(*this->vm, GlobalObject::create(*this->vm, GlobalObject::createStructure(*this->vm, jsNull())));
        }

        ~RuntimeImpl() {
            JSLockHolder lock(*this->vm);
            this->globalObject.clear();
            this->vm.clear();
        }
    };

    Runtime::Runtime(std::wstring applicationPath) : Runtime(new RuntimeImpl(), applicationPath) {
    }

    Runtime::Runtime(RuntimeImplRef impl, std::wstring applicationPath)
        : applicationPath(applicationPath)
        , globalContext(toGlobalRef(impl->globalObject->globalExec()))
        , _impl(impl) {
        wtfThreadData().m_apiData = static_cast<void*>(this);
    }

    Runtime::~Runtime() {
        delete this->_impl;
    }

    void Runtime::initialize() {
        initializeThreading();
    }

    void Runtime::executeModule(const wchar_t* moduleIdentifier, JSValueRef** error) {
    }
}
