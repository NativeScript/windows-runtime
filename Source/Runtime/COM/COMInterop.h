#pragma once

#include "COMObjectMap.h"
#include "../FFI/FFIInterop.h"
#include <cor.h>
#include <MetadataReader.h>

namespace NativeScript {
class COMUnknownType;
class HSTRINGType;

class COMInterop : public FFIInterop {
    typedef FFIInterop Base;

    DECLARE_INFO;

public:
    static COMInterop* create(JSC::VM& vm, JSC::Structure* structure) {
        COMInterop* object = new (NotNull, JSC::allocateCell<COMInterop>(vm.heap)) COMInterop(vm, structure);
        object->finishCreation(vm);
        vm.heap.addFinalizer(object, destroy);
        return object;
    }

    static const bool needsDestruction = false;

    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype) {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

    JSC::JSValue wrap(IUnknown*, JSC::Structure* wrapperStructure = nullptr);

    HRESULT wrap(JSC::JSValue, REFIID, IUnknown**);

    COMObjectMap& objectMap() {
        return _objectMap;
    }

    JSC::JSObject* resolveType(HSTRING);
    JSC::JSObject* resolveType(IMetaDataImport2*, PCCOR_SIGNATURE);

    JSC::Structure* methodCallStructure() const {
        return _methodCallStructure.get();
    }

    JSC::Structure* constructorCallStructure() const {
        return _constructorCallStructure.get();
    }

    ~COMInterop() = default;

private:
    COMInterop(JSC::VM& vm, JSC::Structure* structure)
        : Base(vm, structure) {
    }

    void finishCreation(JSC::VM&);

    static void visitChildren(JSC::JSCell*, JSC::SlotVisitor&);

    static void destroy(JSC::JSCell*);

    Metadata::MetadataReader _metadataReader;

    COMObjectMap _objectMap;
    JSC::WeakGCMap<WTF::String, JSC::JSObject> _typeCache;
    JSC::WriteBarrier<JSC::Structure> _constructorStructure;
    JSC::WriteBarrier<JSC::Structure> _objectWrapperStructure;
    JSC::WriteBarrier<JSC::Structure> _methodCallStructure;
    JSC::WriteBarrier<JSC::Structure> _constructorCallStructure;
    JSC::WriteBarrier<COMUnknownType> _unknownType;
    JSC::WriteBarrier<HSTRINGType> _hstringType;
};
}