#pragma once

#include <wrl.h>

#include <windows.data.json.h>
#include <windows.ui.xaml.data.h>
#include <windows.ui.xaml.interop.h>

#include "CxxGameraww_h.h"
#include "JsonProperty.h"
#include "JsonVectorView.h"

namespace CxxGameraww {

class JsonPropertyProvider : public Microsoft::WRL::RuntimeClass<
        ABI::Windows::UI::Xaml::Data::ICustomPropertyProvider,
        ABI::CxxGameraww::IJsonPropertyProvider
    > {
    InspectableClass(L"CxxGameraww.JsonPropertyProvider", BaseTrust);

public:
    explicit JsonPropertyProvider(ABI::Windows::Data::Json::IJsonObject* object);

    virtual HRESULT STDMETHODCALLTYPE GetCustomProperty(
        /* [in] */ __RPC__in HSTRING name,
        /* [out][retval] */ __RPC__deref_out_opt ABI::Windows::UI::Xaml::Data::ICustomProperty** returnValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetIndexedProperty(
        /* [in] */ __RPC__in HSTRING name,
        /* [in] */ ABI::Windows::UI::Xaml::Interop::TypeName type,
        /* [out][retval] */ __RPC__deref_out_opt ABI::Windows::UI::Xaml::Data::ICustomProperty** returnValue) override;

    virtual HRESULT STDMETHODCALLTYPE GetStringRepresentation(
        /* [out][retval] */ __RPC__deref_out_opt HSTRING* returnValue) override;

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Type(
        /* [out][retval] */ __RPC__out ABI::Windows::UI::Xaml::Interop::TypeName* value) override;

    virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Object(
        /* [out][retval] */ IInspectable** value) override;

private:
    Microsoft::WRL::ComPtr<ABI::Windows::Data::Json::IJsonObject> _object;
};

}
