#include "Runtime-Prefix.h"
#include "COMInterop.h"
#include "COMMethodCall.h"
#include "COMConstructor.h"
#include "HSTRINGType.h"
#include "COMUnknownType.h"
#include <JavaScriptCore/Weak.h>
#include <JavaScriptCore/FunctionPrototype.h>
#include <Signature.h>

namespace NativeScript {
using namespace JSC;

const ClassInfo COMInterop::s_info = { "COMInterop", &Base::s_info, nullptr, CREATE_METHOD_TABLE(COMInterop) };

void COMInterop::finishCreation(VM& vm) {
    Base::finishCreation(vm);

    JSGlobalObject* globalObject = this->globalObject();

    _constructorStructure.set(vm, this, COMConstructor::createStructure(vm, globalObject, globalObject->functionPrototype()));
    _objectWrapperStructure.set(vm, this, COMObjectWrapper::createStructure(vm, globalObject, globalObject->objectPrototype()));
    _methodCallStructure.set(vm, this, COMMethodCall::createStructure(vm, globalObject, globalObject->functionPrototype()));
    _constructorCallStructure.set(vm, this, COMConstructorCall::createStructure(vm, globalObject, jsNull()));
    _unknownType.set(vm, this, COMUnknownType::create(vm, COMUnknownType::createStructure(vm, globalObject, globalObject->objectPrototype())));
    _hstringType.set(vm, this, HSTRINGType::create(vm, HSTRINGType::createStructure(vm, globalObject, globalObject->objectPrototype())));
}

JSValue COMInterop::wrap(IUnknown* unknown, Structure* wrapperStructure) {
    if (!unknown) {
        return jsNull();
    }

    Microsoft::WRL::ComPtr<IUnknown> definiteUnknown;
    unknown->QueryInterface(definiteUnknown.GetAddressOf());

    if (COMObjectWrapper* existingWrapper = _objectMap.get(definiteUnknown.Get())) {
        return existingWrapper;
    }

    Microsoft::WRL::ComPtr<IInspectable> inspectable;
    if (SUCCEEDED(unknown->QueryInterface(inspectable.GetAddressOf()))) {
        Microsoft::WRL::Wrappers::HString className;
        inspectable->GetRuntimeClassName(className.GetAddressOf());
        JSObject* type = resolveType(className.Get());

        if (COMConstructor* ctor = jsDynamicCast<COMConstructor*>(type)) {
            wrapperStructure = ctor->instanceStructure();
        } else {
            // TODO: handle interfaces
        }
    }

    if (!wrapperStructure) {
        // default to the empty, IUnknown structure
        wrapperStructure = _objectWrapperStructure.get();
    }

    COMObjectWrapper* wrapper = COMObjectWrapper::create(globalObject()->vm(), wrapperStructure, definiteUnknown.Get());
    _objectMap.set(wrapper);
    return wrapper;
}

HRESULT COMInterop::wrap(JSValue value, REFIID target, IUnknown** buffer) {
    if (COMObjectWrapper* wrapper = jsDynamicCast<COMObjectWrapper*>(value)) {
        return wrapper->wrappedObject()->QueryInterface(target, reinterpret_cast<void**>(buffer));
    } else if (COMConstructor* ctor = jsDynamicCast<COMConstructor*>(value)) {
        return ctor->factory()->QueryInterface(target, reinterpret_cast<void**>(buffer));
    }

    return E_NOTIMPL;
}

JSObject* COMInterop::resolveType(HSTRING typeName) {
    unsigned typeNameLength;
    const wchar_t* typeNameRaw = WindowsGetStringRawBuffer(typeName, &typeNameLength);

    JSObject* type;
    if (type = _typeCache.get(WTF::StringImpl::createWithoutCopying(typeNameRaw, typeNameLength))) {
        return type;
    }

    std::shared_ptr<Metadata::Declaration> decl = _metadataReader.findByName(typeName);
    if (decl && decl->kind() == Metadata::DeclarationKind::Class) {
        Microsoft::WRL::ComPtr<IUnknown> classFactory;
        HRESULT hr = RoGetActivationFactory(typeName, IID_IUnknown, reinterpret_cast<void**>(classFactory.GetAddressOf()));
        ASSERT(SUCCEEDED(hr));

        type = COMConstructor::create(globalObject()->vm(), _constructorStructure.get(), this, classFactory.Get(), static_pointer_cast<Metadata::ClassDeclaration>(decl));
    }
    // TODO: construct type from decl

    if (!type) {
        type = _unknownType.get();
    }

    return type;
}

JSObject* COMInterop::resolveType(IMetaDataImport2* metadata, PCCOR_SIGNATURE signature) {
    CorElementType elementType{ CorSigUncompressElementType(signature) };
    switch (elementType) {
    case ELEMENT_TYPE_VOID:
        return _voidType.get();
    case ELEMENT_TYPE_BOOLEAN:
        return _boolType.get();
    case ELEMENT_TYPE_CHAR:
        return nullptr;
    case ELEMENT_TYPE_I1:
        return _int8Type.get();
    case ELEMENT_TYPE_U1:
        return _uint8Type.get();
    case ELEMENT_TYPE_I2:
        return _int16Type.get();
    case ELEMENT_TYPE_U2:
        return _uint16Type.get();
    case ELEMENT_TYPE_I4:
        return _int32Type.get();
    case ELEMENT_TYPE_U4:
        return _uint32Type.get();
    case ELEMENT_TYPE_I8:
        return _int64Type.get();
    case ELEMENT_TYPE_U8:
        return _uint64Type.get();
    case ELEMENT_TYPE_R4:
        return _floatType.get();
    case ELEMENT_TYPE_R8:
        return _doubleType.get();
    case ELEMENT_TYPE_STRING:
        return _hstringType.get();
    case ELEMENT_TYPE_OBJECT:
        return _unknownType.get();
    case ELEMENT_TYPE_BYREF:
        return nullptr;
    default:
        std::wstring typeName = Metadata::Signature::toString(metadata, signature - 1);
        return resolveType(Microsoft::WRL::Wrappers::HStringReference(typeName.c_str(), typeName.size()).Get());
    }
}

void COMInterop::visitChildren(JSCell* cell, SlotVisitor& slotVisitor) {
    Base::visitChildren(cell, slotVisitor);

    COMInterop* interop = jsCast<COMInterop*>(cell);

    slotVisitor.append(&interop->_constructorStructure);
    slotVisitor.append(&interop->_objectWrapperStructure);
    slotVisitor.append(&interop->_methodCallStructure);
    slotVisitor.append(&interop->_constructorCallStructure);
    slotVisitor.append(&interop->_unknownType);
    slotVisitor.append(&interop->_hstringType);
}

void COMInterop::destroy(JSCell* cell) {
    static_cast<COMInterop*>(cell)->~COMInterop();
}
}