#include "Runtime-Prefix.h"
#include "COMConstructor.h"
#include "../GlobalObject.h"
#include "COMMethodCall.h"
#include "COMObjectWrapper.h"
#include <JavaScriptCore/ObjectConstructor.h>
#include <Declarations/InterfaceDeclaration.h>
#include <DeclaringInterfaceForMethod.h>
#include <algorithm>
#include <comdef.h>

namespace NativeScript {
using namespace JSC;

const ClassInfo COMConstructorCall::s_info = { "COMConstructorCall", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMConstructorCall) };

void COMConstructorCall::finishCreation(VM& vm, COMInterop* interop, const Metadata::MethodDeclaration& initializer, Initialization initialization) {
    Base::finishCreation(vm);

    _initialization = initialization;

    if (auto factoryInterface = Metadata::findDeclaringInterfaceForMethod(initializer, &_methodIndex)) {
        _methodInterface = factoryInterface->id();
    } else {
        _methodInterface = IID_IActivationFactory;
        _methodIndex = 0;
    }

    _methodIndex += 6; // account for IInspectable vtable overhead

    size_t numberOfParameters = initializer.numberOfParameters();
    size_t numberOfABIParameters = numberOfParameters + (initialization == Activation ? 2 : 4);

    _parameterCells.reserveCapacity(numberOfParameters);
    _parameterTypes.reserveCapacity(numberOfABIParameters);
    _parameterTypes.append(&ffi_type_pointer);

    for (auto& parameter : initializer.parameters()) {
        JSObject* parameterType = interop->resolveType(parameter.metadata(), parameter.type());
        _parameterCells.append(WriteBarrier<JSCell>(vm, this, parameterType));
        _parameterTypes.append(getFFIMethodTable(parameterType)->getFFITypeStruct(parameterType));
    }

    if (initialization == Composition) {
        _parameterTypes.append(&ffi_type_pointer);
        _parameterTypes.append(&ffi_type_pointer);
    }

    _parameterTypes.append(&ffi_type_pointer);

    ffi_prep_cif(&_cif, FFI_STDCALL, numberOfABIParameters, &ffi_type_sint32, _parameterTypes.data());
}

JSValue COMConstructorCall::construct(ExecState* execState) {
    COMConstructor* ctor = jsCast<COMConstructor*>(execState->callee());

    size_t numberOfABIParameters = _parameterCells.size() + (_initialization == Activation ? 2 : 4);
    ;
    IUnknown* result = nullptr;
    IUnknown** resultPtr = &result;

    Microsoft::WRL::ComPtr<IUnknown> activationFactory;
    ctor->factory()->QueryInterface(_methodInterface, reinterpret_cast<void**>(activationFactory.GetAddressOf()));
    ASSERT(activationFactory.Get());

    void** vtable = *reinterpret_cast<void***>(activationFactory.Get());
    void* fn = vtable[_methodIndex];

    WTF::Vector<void*> arguments;
    arguments.reserveCapacity(numberOfABIParameters);

    ReleasePoolHolder releasePool;

    IUnknown* self = activationFactory.Get();
    arguments.append(&self);
    for (size_t i = 0; i < _parameterCells.size(); i++) {
        JSCell* type = _parameterCells[i].get();
        void* buffer = _alloca(std::max(sizeof(ffi_arg), _parameterTypes[i + 1]->size));
        getFFIMethodTable(type)->marshalJSToNative(type, execState, execState->uncheckedArgument(i), buffer);
        arguments.append(buffer);
    }

    if (_initialization == Composition) {
        // TODO: handle inheritance
        void* null = nullptr;
        arguments.append(&null);
        arguments.append(&null);
    }

    arguments.append(&resultPtr);

    HRESULT hr;
    ffi_call(&_cif, FFI_FN(fn), &hr, arguments.data());

    if (!SUCCEEDED(hr)) {
        _com_error error(hr, nullptr);
        return execState->vm().throwException(execState, createError(execState, error.ErrorMessage()));
    } else {
        COMInterop* interop = jsCast<GlobalObject*>(execState->lexicalGlobalObject())->interop();
        return interop->wrap(result, ctor->instanceStructure());
    }
}

void COMConstructorCall::visitChildren(JSCell* cell, SlotVisitor& visitor) {
    Base::visitChildren(cell, visitor);

    COMConstructorCall* call = jsCast<COMConstructorCall*>(cell);
    visitor.append(call->_parameterCells.begin(), call->_parameterCells.end());
}

void COMConstructorCall::destroy(JSCell* cell) {
    jsCast<COMConstructorCall*>(cell)->COMConstructorCall::~COMConstructorCall();
}

const FFITypeClassInfo COMConstructor::s_info = { "COMConstructor", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMConstructor), CREATE_FFI_METHOD_TABLE(COMConstructor) };

void COMConstructor::finishCreation(VM& vm, COMInterop* interop, IUnknown* factory, std::shared_ptr<Metadata::ClassDeclaration>&& klass) {
    Base::finishCreation(vm, WTF::String(klass->name().c_str()));

    JSGlobalObject* globalObject = this->globalObject();
    ExecState* globalExec = globalObject->globalExec();

    _factory = factory;
    _class = klass;

    JSObject* prototype = constructEmptyObject(globalExec);

    _instanceStructure.set(vm, this, COMObjectWrapper::createStructure(vm, globalObject, prototype));

    for (auto& method : klass->methods()) {
        COMMethodCall* methodCall = COMMethodCall::create(vm, interop, method);
        Identifier methodName(&vm, methodCall->name(globalExec));

        if (method.isStatic()) {
            putDirect(vm, methodName, methodCall);
        } else {
            prototype->putDirect(vm, methodName, methodCall);
        }
    }

    COMConstructorCall::Initialization classInitialization = klass->isSealed() ? COMConstructorCall::Activation : COMConstructorCall::Composition;
    for (auto& initializer : klass->initializers()) {
        COMConstructorCall* constructorCall = COMConstructorCall::create(vm, interop, initializer, classInitialization);
        WriteBarrier<COMConstructorCall> initializerBarrier(vm, this, constructorCall);
        _initializers.emplace(initializer.numberOfParameters(), std::move(initializerBarrier));
    }

    //for (auto& prop : klass->properties()) {
    //    PropertyDescriptor descriptor;
    //    descriptor.setConfigurable(true);
    //    descriptor.setGetter(COMMethodCall::create(vm, methodCallStructure, prop.getter()));

    //    Identifier propertyName(&vm, prop.name().c_str());

    //    if (const Metadata::MethodDeclaration* setter = prop.setter()) {
    //        descriptor.setSetter(COMMethodCall::create(vm, methodCallStructure, *setter));
    //    }

    //    if (prop.isStatic()) {
    //        defineOwnProperty(this, globalExec, propertyName, descriptor, false);
    //    } else {
    //        prototype->defineOwnProperty(prototype, globalExec, propertyName, descriptor, false);
    //    }
    //}

    prototype->putDirect(vm, vm.propertyNames->constructor, this, ReadOnly | DontDelete);
    putDirect(vm, vm.propertyNames->prototype, prototype, ReadOnly | DontDelete);
}

CallType COMConstructor::getCallData(JSCell* cell, CallData& callData) {
    return CallTypeNone;
}

ConstructType COMConstructor::getConstructData(JSCell* cell, ConstructData& constructData) {
    COMConstructor* ctor = jsCast<COMConstructor*>(cell);
    if (ctor->_class->isInstantiable()) {
        constructData.native.function = &construct;
        return ConstructTypeHost;
    }

    return ConstructTypeNone;
}

EncodedJSValue JSC_HOST_CALL COMConstructor::construct(ExecState* execState) {
    COMConstructor* ctor = jsCast<COMConstructor*>(execState->callee());

    auto& initializer = ctor->_initializers.find(execState->argumentCount());
    if (initializer != ctor->_initializers.end()) {
        COMConstructorCall* constructorCall = initializer->second.get();
        return JSValue::encode(constructorCall->construct(execState));
    }

    return JSValue::encode(execState->vm().throwException(execState, createError(execState, L"Could not find viable overload for constructor")));
}

void COMConstructor::visitChildren(JSCell* cell, SlotVisitor& slotVisitor) {
    Base::visitChildren(cell, slotVisitor);

    COMConstructor* ctor = jsCast<COMConstructor*>(cell);

    slotVisitor.append(&ctor->_instanceStructure);

    for (auto& iter : ctor->_initializers) {
        slotVisitor.append(&iter.second);
    }
}

void COMConstructor::destroy(JSCell* cell) {
    jsCast<COMConstructor*>(cell)->COMConstructor::~COMConstructor();
}

JSValue COMConstructor::marshalNativeToJS(const JSCell* cell, ExecState* execState, const void* buffer) {
    const COMConstructor* ctor = jsCast<const COMConstructor*>(cell);
    COMInterop* interop = jsCast<GlobalObject*>(execState->lexicalGlobalObject())->interop();

    return interop->wrap(*reinterpret_cast<IUnknown**>(const_cast<void*>(buffer)), ctor->instanceStructure());
}

void COMConstructor::marshalJSToNative(const JSCell* cell, ExecState* execState, JSValue value, void* buffer) {
    const COMConstructor* ctor = jsCast<const COMConstructor*>(cell);
    COMInterop* interop = jsCast<GlobalObject*>(execState->lexicalGlobalObject())->interop();

    interop->wrap(value, ctor->_class->defaultInterface().id(), reinterpret_cast<IUnknown**>(buffer));
}

ffi_type* COMConstructor::getFFITypeStruct(const JSCell* cell) {
    return &ffi_type_pointer;
}
}