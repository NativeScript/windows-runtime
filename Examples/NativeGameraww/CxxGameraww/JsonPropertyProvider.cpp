#include "pch.h"

#include "JsonPropertyProvider.h"

namespace CxxGameraww {

using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Xaml::Data;
using namespace ABI::Windows::UI::Xaml::Interop;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Data::Json;

JsonPropertyProvider::JsonPropertyProvider(IJsonObject* object)
    : _object(object) {
}

STDMETHODIMP JsonPropertyProvider::GetCustomProperty(HSTRING name, ICustomProperty** returnValue) {
    ComPtr<IMap<HSTRING, IJsonValue*>> map;
    ASSERT_SUCCESS(_object.As(&map));

    boolean hasKey = false;
    ASSERT_SUCCESS(map->HasKey(name, &hasKey));

    if (hasKey) {
        ComPtr<JsonProperty> jsonProperty = Make<JsonProperty>(name);
        return jsonProperty.Detach()->QueryInterface(IID_ICustomProperty, reinterpret_cast<void**>(returnValue));
    }

    *returnValue = nullptr;
    return S_OK;
}

STDMETHODIMP JsonPropertyProvider::GetIndexedProperty(HSTRING name, TypeName type, ICustomProperty** returnValue) {
    *returnValue = nullptr;
    return S_OK;
}

STDMETHODIMP JsonPropertyProvider::GetStringRepresentation(HSTRING* returnValue) {
    ComPtr<IJsonValue> value;
    ASSERT_SUCCESS(_object.As(&value));
    return value->Stringify(returnValue);
}

STDMETHODIMP JsonPropertyProvider::get_Type(TypeName* value) {
    value->Kind = TypeKind_Custom;
    return GetRuntimeClassName(&value->Name);
}

STDMETHODIMP JsonPropertyProvider::get_Object(IInspectable** value) {
    return _object.CopyTo(value);
}

}
