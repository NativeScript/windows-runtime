#include "pch.h"
#include <JavaScriptCore/InitializeThreading.h>
#include <JavaScriptCore/APICast.h>
#include <JavaScriptCore/FunctionConstructor.h>
#include <JavaScriptCore/JSGlobalObjectInspectorController.h>
#include <JavaScriptCore/StrongInlines.h>
#include "GlobalObject.h"

#include <MetadataReader.h>

#include "Runtime.h"

namespace NativeScript {
using namespace std;
using namespace JSC;
using namespace Metadata;

namespace {
    MetadataReader metadataReader;

    EncodedJSValue JSC_HOST_CALL getMetadata(ExecState* execState) {
        WTF::String typeName = execState->argument(0).toWTFString(execState);
        shared_ptr<Declaration> declaration{ metadataReader.findByName(typeName.createHString().Get()) };

        if (!declaration) {
            return JSValue::encode(jsNull());
        }

        return JSValue::encode(jsString(execState, declaration->fullName().data()));
    }
}

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

Runtime::Runtime(const wchar_t* applicationPath)
    : Runtime(new RuntimeImpl(), applicationPath) {
}

Runtime::Runtime(RuntimeImplRef impl, const wchar_t* applicationPath)
    : applicationPath(applicationPath)
    , globalContext(toGlobalRef(impl->globalObject->globalExec()))
    , _impl(impl) {
    wtfThreadData().m_apiData = static_cast<void*>(this);

    GlobalObject* globalObject = impl->globalObject.get();

    globalObject->putDirectNativeFunction(*impl->vm, globalObject, Identifier(globalObject->globalExec(), WTF::ASCIILiteral("T")), 1, &getMetadata, NoIntrinsic, DontEnum | Attribute::Function);
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
