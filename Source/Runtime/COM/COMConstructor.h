#pragma once

#include <JavaScriptCore/InternalFunction.h>
#include <Declarations/ClassDeclaration.h>
#include "../FFI/FFIType.h"
#include "COMInterop.h"
#include <map>

namespace NativeScript {
class COMConstructorCall;

class COMConstructor : public JSC::InternalFunction {
    typedef JSC::InternalFunction Base;

    DECLARE_FFI_INFO

public:
    static COMConstructor* create(JSC::VM& vm, JSC::Structure* structure, COMInterop* interop, IUnknown* factory, std::shared_ptr<Metadata::ClassDeclaration>&& klass) {
        COMConstructor* object = new (NotNull, JSC::allocateCell<COMConstructor>(vm.heap)) COMConstructor(vm, structure);
        object->finishCreation(vm, interop, factory, std::move(klass));
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    static const unsigned StructureFlags = IsFFITypeFlag | Base::StructureFlags;

    static const bool needsDestruction = false;

    IUnknown* factory() const {
        return _factory.Get();
    }

    JSC::Structure* instanceStructure() const {
        return _instanceStructure.get();
    }

private:
    COMConstructor(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    static JSC::CallType getCallData(JSC::JSCell*, JSC::CallData&);

    static JSC::ConstructType getConstructData(JSC::JSCell*, JSC::ConstructData&);

    static JSC::EncodedJSValue JSC_HOST_CALL construct(JSC::ExecState*);

    void finishCreation(JSC::VM&, COMInterop*, IUnknown*, std::shared_ptr<Metadata::ClassDeclaration>&&);

    static void visitChildren(JSC::JSCell*, JSC::SlotVisitor&);

    static void destroy(JSC::JSCell*);

    static JSC::JSValue marshalNativeToJS(const JSC::JSCell*, JSC::ExecState*, const void*);

    static void marshalJSToNative(const JSC::JSCell*, JSC::ExecState*, JSC::JSValue, void*);

    static ffi_type* getFFITypeStruct(const JSCell*);

    ~COMConstructor() = default;

    Microsoft::WRL::ComPtr<IUnknown> _factory;
    std::shared_ptr<Metadata::ClassDeclaration> _class;

    JSC::WriteBarrier<JSC::Structure> _instanceStructure;
    std::map<size_t, JSC::WriteBarrier<COMConstructorCall>> _initializers;
};

class COMConstructorCall : public JSC::JSNonFinalObject {
    typedef JSC::JSNonFinalObject Base;

    DECLARE_INFO

public:
    enum Initialization {
        Activation,
        Composition
    };

    static COMConstructorCall* create(JSC::VM& vm, COMInterop* interop, const Metadata::MethodDeclaration& initializer, Initialization initialization) {
        COMConstructorCall* object = new (NotNull, JSC::allocateCell<COMConstructorCall>(vm.heap)) COMConstructorCall(vm, interop->constructorCallStructure());
        object->finishCreation(vm, interop, initializer, initialization);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    static const bool needsDestruction = false;

    JSC::JSValue construct(JSC::ExecState*);

private:
    COMConstructorCall(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    void finishCreation(JSC::VM&, COMInterop*, const Metadata::MethodDeclaration&, Initialization);

    static void visitChildren(JSC::JSCell*, JSC::SlotVisitor&);

    static void destroy(JSC::JSCell*);

    ~COMConstructorCall() = default;

    Initialization _initialization;
    size_t _methodIndex;
    IID _methodInterface;
    ffi_cif _cif;
    WTF::Vector<ffi_type*> _parameterTypes;
    WTF::Vector<JSC::WriteBarrier<JSC::JSCell>> _parameterCells;
};
}