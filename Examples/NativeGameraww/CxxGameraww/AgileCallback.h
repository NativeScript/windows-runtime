#pragma once

#include <wrl.h>

namespace CxxGameraww {

template <typename TDelegateInterface, typename TCallback, unsigned int argCount>
struct AgileInvokeHelper;

template <typename TDelegateInterface, typename TCallback>
struct AgileInvokeHelper<TDelegateInterface, TCallback, 0>
    WrlSealed : public Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::Delegate>, TDelegateInterface, IAgileObject> {
    explicit AgileInvokeHelper(TCallback callback) throw() : callback_(callback) {
    }

    STDMETHOD(Invoke)() {
        return Microsoft::WRL::DelegateTraits<Microsoft::WRL::DefaultDelegateCheckMode>::CheckReturn(callback_());
    }

    TCallback callback_;
};

template <typename TDelegateInterface, typename TCallback>
Microsoft::WRL::ComPtr<typename Microsoft::WRL::Details::ArgTraitsHelper<TDelegateInterface>::Interface> AgileCallback(TCallback callback) throw() {
    static_assert(__is_base_of(IUnknown, TDelegateInterface) && !__is_base_of(IInspectable, TDelegateInterface), "Delegates objects must be 'IUnknown' base and not 'IInspectable'");

    return Make<AgileInvokeHelper<TDelegateInterface, TCallback,
        Microsoft::WRL::Details::ArgTraitsHelper<TDelegateInterface>::args>>(callback);
}

}
