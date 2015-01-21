#pragma once

#include "COMObjectWrapper.h"

namespace NativeScript {
class COMObjectMap {
public:
    COMObjectWrapper* get(IUnknown* unknown) {
        ASSERT(unknown);

#if !defined(NDEBUG)
        Microsoft::WRL::ComPtr<IUnknown> definiteUnknown;
        unknown->QueryInterface(definiteUnknown.GetAddressOf());
        ASSERT(definiteUnknown.Get() == unknown);
#endif

        return _wrappedObjects.get(unknown);
    }

    void set(COMObjectWrapper* wrapper) {
        ASSERT(wrapper);

        _wrappedObjects.set(wrapper->wrappedObject().Get(), wrapper);
    }

    void remove(COMObjectWrapper* wrapper) {
        ASSERT(wrapper);

        _wrappedObjects.remove(wrapper->wrappedObject().Get());
    }

private:
    JSC::WeakGCMap<IUnknown*, COMObjectWrapper> _wrappedObjects;
};
}