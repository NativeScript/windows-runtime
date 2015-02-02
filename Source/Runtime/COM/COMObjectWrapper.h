#pragma once

#include <JavaScriptCore/JSObject.h>

namespace NativeScript {
class COMObjectWrapper : public JSC::JSNonFinalObject {
public:
    typedef JSC::JSNonFinalObject Base;

    static COMObjectWrapper* create(JSC::VM& vm, JSC::Structure* structure, IUnknown* wrappedObject) {
        COMObjectWrapper* object = new (NotNull, JSC::allocateCell<COMObjectWrapper>(vm.heap)) COMObjectWrapper(vm, structure);
        object->finishCreation(vm, wrappedObject);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static const bool needsDestruction = false;

    DECLARE_INFO;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    IUnknown* wrappedObject() const {
        return _wrappedObject.Get();
    }

    static WTF::String className(const JSC::JSObject* object);

    ~COMObjectWrapper();

private:
    COMObjectWrapper(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static void destroy(JSC::JSCell* cell) {
        static_cast<COMObjectWrapper*>(cell)->~COMObjectWrapper();
    }

    void finishCreation(JSC::VM& vm, IUnknown* wrappedObject);

    Microsoft::WRL::ComPtr<IUnknown> _wrappedObject;
};
}