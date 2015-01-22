#include "Runtime-Prefix.h"
#include "FFIInterop.h"

namespace NativeScript {
using namespace JSC;

const ClassInfo FFIInterop::s_info = { "FFIInterop", &Base::s_info, nullptr, CREATE_METHOD_TABLE(FFIInterop) };

void FFIInterop::finishCreation(JSC::VM& vm) {
    Base::finishCreation(vm);

    JSGlobalObject* globalObject = this->globalObject();
    JSValue typePrototype = globalObject->objectPrototype();

    _voidType.set(vm, this, FFIVoidType::create(vm, FFIVoidType::createStructure(vm, globalObject, typePrototype)));
    _int8Type.set(vm, this, FFINumberType<int8_t>::create(vm, FFINumberType<int8_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_sint8));
    _uint8Type.set(vm, this, FFINumberType<uint8_t>::create(vm, FFINumberType<uint8_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_uint8));
    _int16Type.set(vm, this, FFINumberType<int16_t>::create(vm, FFINumberType<int16_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_sint16));
    _uint16Type.set(vm, this, FFINumberType<uint16_t>::create(vm, FFINumberType<uint16_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_uint16));
    _int32Type.set(vm, this, FFINumberType<int32_t>::create(vm, FFINumberType<int32_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_sint32));
    _uint32Type.set(vm, this, FFINumberType<uint32_t>::create(vm, FFINumberType<uint32_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_uint32));
    _int64Type.set(vm, this, FFINumberType<int64_t>::create(vm, FFINumberType<int64_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_sint64));
    _uint64Type.set(vm, this, FFINumberType<uint64_t>::create(vm, FFINumberType<uint64_t>::createStructure(vm, globalObject, typePrototype), &ffi_type_uint64));
    _floatType.set(vm, this, FFINumberType<float>::create(vm, FFINumberType<float>::createStructure(vm, globalObject, typePrototype), &ffi_type_float));
    _doubleType.set(vm, this, FFINumberType<double>::create(vm, FFINumberType<double>::createStructure(vm, globalObject, typePrototype), &ffi_type_double));
    _boolType.set(vm, this, FFIBooleanType<bool>::create(vm, FFIBooleanType<bool>::createStructure(vm, globalObject, typePrototype)));
}

void FFIInterop::visitChildren(JSC::JSCell* cell, JSC::SlotVisitor& visitor) {
    Base::visitChildren(cell, visitor);

    FFIInterop* interop = jsCast<FFIInterop*>(cell);
    visitor.append(&interop->_voidType);
    visitor.append(&interop->_int8Type);
    visitor.append(&interop->_uint8Type);
    visitor.append(&interop->_int16Type);
    visitor.append(&interop->_uint16Type);
    visitor.append(&interop->_int32Type);
    visitor.append(&interop->_uint32Type);
    visitor.append(&interop->_int64Type);
    visitor.append(&interop->_uint64Type);
    visitor.append(&interop->_floatType);
    visitor.append(&interop->_doubleType);
    visitor.append(&interop->_boolType);
}

void FFIInterop::destroy(JSCell* cell) {
    jsCast<FFIInterop*>(cell)->FFIInterop::~FFIInterop();
}
}