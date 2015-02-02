#include "Runtime-Prefix.h"
#include "COMMethodCall.h"
#include "COMInterop.h"
#include "../GlobalObject.h"
#include <DeclaringInterfaceForMethod.h>
#include <algorithm>
#include <comdef.h>

namespace NativeScript {
using namespace JSC;

const ClassInfo COMMethodCall::s_info = { "COMMethodCall", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMMethodCall) };

void COMMethodCall::finishCreation(VM& vm, COMInterop* interop, const Metadata::MethodDeclaration& method) {
    std::wstring name = method.overloadName();
    if (name.empty()) {
        name = method.name();
    }
    Base::finishCreation(vm, WTF::String(name.c_str()));

    auto iface = Metadata::findDeclaringInterfaceForMethod(method, &_methodIndex);
    _methodInterface = iface->id();
    _methodIndex += 6;

    _isVoid = false;

    auto parameters = method.parameters();
    size_t numberOfParameters = method.numberOfParameters();
    size_t numberOfABIParameters = numberOfParameters + (_isVoid ? 1 : 2);
    _parameterCells.reserveCapacity(numberOfParameters);

    _parameterTypes = new ffi_type* [numberOfABIParameters];
    unsigned i = 1;
    for (const Metadata::ParameterDeclaration& parameter : parameters) {
        JSObject* parameterType = interop->resolveType(parameter.metadata(), parameter.type());
        _parameterCells.append(WriteBarrier<JSCell>(vm, this, parameterType));
        _parameterTypes[i] = getFFIMethodTable(parameterType)->getFFITypeStruct(parameterType);
        i++;
    }

    _parameterTypes[0] = &ffi_type_pointer;
    if (!_isVoid) {
        _parameterTypes[numberOfABIParameters - 1] = &ffi_type_pointer;
        _returnType.set(vm, this, interop->resolveType(method.metadata(), method.returnType()));
    }

    ffi_prep_cif(&_cif, FFI_STDCALL, numberOfABIParameters, &ffi_type_sint32, _parameterTypes);
}

CallType COMMethodCall::getCallData(JSCell* cell, CallData& callData) {
    callData.native.function = &call;
    return CallTypeHost;
}

EncodedJSValue JSC_HOST_CALL COMMethodCall::call(ExecState* execState) {
    COMMethodCall* callee = jsCast<COMMethodCall*>(execState->callee());
    if (execState->argumentCount() != callee->_parameterCells.size()) {
        // TODO: error
        CRASH();
    }

    COMInterop* interop = jsCast<GlobalObject*>(execState->lexicalGlobalObject())->interop();

    size_t numberOfABIParameters = callee->_parameterCells.size() + (callee->_isVoid ? 1 : 2);

    HRESULT hr;

    Microsoft::WRL::ComPtr<IUnknown> self;
    hr = interop->wrap(execState->thisValue(), callee->_methodInterface, self.GetAddressOf());

    void** vtable = *reinterpret_cast<void***>(self.Get());
    void* fn = vtable[callee->_methodIndex];

    WTF::Vector<void*> arguments;
    arguments.reserveCapacity(numberOfABIParameters);

    IUnknown* thisValue = self.Get();
    arguments.append(&thisValue);

    for (int i = 0; i < callee->_parameterCells.size(); i++) {
        JSCell* type = callee->_parameterCells[i].get();
        void* buffer = _alloca(std::max(sizeof(ffi_arg), callee->_parameterTypes[i + 1]->size));
        getFFIMethodTable(type)->marshalJSToNative(type, execState, execState->uncheckedArgument(i), buffer);
        arguments.append(buffer);
    }

    void* returnBuffer = nullptr;
    if (!callee->_isVoid) {
        returnBuffer = _alloca(std::max(sizeof(ffi_arg), callee->_parameterTypes[numberOfABIParameters - 1]->size));
        arguments.append(&returnBuffer);
    }

    ffi_call(&callee->_cif, FFI_FN(fn), &hr, arguments.data());

    JSValue jsResult;
    if (!SUCCEEDED(hr)) {
        _com_error error(hr, nullptr);
        jsResult = execState->vm().throwException(execState, createError(execState, error.ErrorMessage()));
    } else if (!callee->_isVoid) {
        JSCell* returnType = callee->_returnType.get();
        jsResult = getFFIMethodTable(returnType)->marshalNativeToJS(returnType, execState, returnBuffer);
    } else {
        jsResult = jsUndefined();
    }

    return JSValue::encode(jsResult);
}

void COMMethodCall::visitChildren(JSCell* cell, SlotVisitor& slotVisitor) {
    Base::visitChildren(cell, slotVisitor);

    COMMethodCall* call = jsCast<COMMethodCall*>(cell);

    slotVisitor.append(call->_parameterCells.begin(), call->_parameterCells.end());
    slotVisitor.append(&call->_returnType);
}

void COMMethodCall::destroy(JSCell* cell) {
    jsCast<COMMethodCall*>(cell)->COMMethodCall::~COMMethodCall();
}

COMMethodCall::~COMMethodCall() {
    delete[] _parameterTypes;
}
}
