#pragma once

#include <JavaScriptCore/InternalFunction.h>
#include <Declarations/MethodDeclaration.h>
#include "../FFI/FFIType.h"
#include "COMInterop.h"

namespace NativeScript {
class COMMethodCall : public JSC::InternalFunction {
    typedef JSC::InternalFunction Base;

    DECLARE_INFO

public:
    static COMMethodCall* create(JSC::VM& vm, COMInterop* interop, const Metadata::MethodDeclaration& method) {
        COMMethodCall* object = new (NotNull, JSC::allocateCell<COMMethodCall>(vm.heap)) COMMethodCall(vm, interop->methodCallStructure());
        object->finishCreation(vm, interop, method);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    static const bool needsDestruction = false;

private:
    friend class COMConstructor;

    COMMethodCall(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static JSC::CallType getCallData(JSC::JSCell*, JSC::CallData&);

    static JSC::EncodedJSValue JSC_HOST_CALL call(JSC::ExecState*);

    void finishCreation(JSC::VM&, COMInterop*, const Metadata::MethodDeclaration&);

    static void visitChildren(JSC::JSCell*, JSC::SlotVisitor&);

    static void destroy(JSC::JSCell*);

    ~COMMethodCall();

    size_t _methodIndex;
    IID _methodInterface;

    ffi_cif _cif;
    bool _isVoid;

    JSC::WriteBarrier<JSC::JSCell> _returnType;

    ffi_type** _parameterTypes;
    WTF::Vector<JSC::WriteBarrier<JSC::JSCell>> _parameterCells;
};
}