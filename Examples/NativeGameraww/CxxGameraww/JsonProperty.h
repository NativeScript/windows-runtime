#pragma once

#include <wrl.h>

#include <windows.ui.xaml.data.h>
#include <windows.ui.xaml.interop.h>
#include <windows.data.json.h>

namespace CxxGameraww {

HRESULT ValueToObject(ABI::Windows::Data::Json::IJsonValue*, IInspectable**);

class JsonProperty : public Microsoft::WRL::RuntimeClass<ABI::Windows::UI::Xaml::Data::ICustomProperty> {
    InspectableClass(L"CxxGameraww.JsonProperty", BaseTrust);

public:
    explicit JsonProperty(HSTRING name);

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Type(
        /* [out][retval] */ __RPC__out ABI::Windows::UI::Xaml::Interop::TypeName* value) override;

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Name(
        /* [out][retval] */ __RPC__deref_out_opt HSTRING* value) override;

    virtual HRESULT STDMETHODCALLTYPE GetValue(
        /* [in] */ __RPC__in_opt IInspectable* target,
        /* [out][retval] */ __RPC__deref_out_opt IInspectable** returnValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetValue(
        /* [in] */ __RPC__in_opt IInspectable* target,
        /* [in] */ __RPC__in_opt IInspectable* value) override;

    virtual HRESULT STDMETHODCALLTYPE GetIndexedValue(
        /* [in] */ __RPC__in_opt IInspectable* target,
        /* [in] */ __RPC__in_opt IInspectable* index,
        /* [out][retval] */ __RPC__deref_out_opt IInspectable** returnValue) override;

    virtual HRESULT STDMETHODCALLTYPE SetIndexedValue(
        /* [in] */ __RPC__in_opt IInspectable* target,
        /* [in] */ __RPC__in_opt IInspectable* value,
        /* [in] */ __RPC__in_opt IInspectable* index) override;

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CanWrite(
        /* [out][retval] */ __RPC__out boolean* value) override;

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_CanRead(
        /* [out][retval] */ __RPC__out boolean* value) override;

private:
    Microsoft::WRL::Wrappers::HString _name;
};

}
