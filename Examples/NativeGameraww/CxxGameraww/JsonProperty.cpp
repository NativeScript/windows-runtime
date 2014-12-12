#include "pch.h"

#include <wrl.h>

#include "JsonProperty.h"
#include "JsonPropertyProvider.h"
#include "JsonVectorView.h"
#include "CxxGameraww_h.h"

namespace CxxGameraww {

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

using namespace ABI::Windows::UI::Xaml::Data;
using namespace ABI::Windows::UI::Xaml::Interop;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Data::Json;
using namespace ABI::Windows::Foundation;

HRESULT ValueToObject(IJsonValue* value, IInspectable** returnValue) {
    JsonValueType valueType;
    ASSERT_SUCCESS(value->get_ValueType(&valueType));

    switch (valueType) {
        case JsonValueType_Null:
            *returnValue = nullptr;
            return S_OK;

        case JsonValueType_Boolean: {
            boolean booleanValue = false;
            ASSERT_SUCCESS(value->GetBoolean(&booleanValue));

            ComPtr<IPropertyValueStatics> propertyValueStatics;
            ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(), &propertyValueStatics));

            return propertyValueStatics->CreateBoolean(booleanValue, returnValue);
        }

        case JsonValueType_Number: {
            double numberValue = 0;
            ASSERT_SUCCESS(value->GetNumber(&numberValue));

            ComPtr<IPropertyValueStatics> propertyValueStatics;
            ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(), &propertyValueStatics));

            return propertyValueStatics->CreateDouble(numberValue, returnValue);
        }

        case JsonValueType_String: {
            HSTRING stringValue = nullptr;
            ASSERT_SUCCESS(value->GetString(&stringValue));

            ComPtr<IPropertyValueStatics> propertyValueStatics;
            ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_PropertyValue).Get(), &propertyValueStatics));

            return propertyValueStatics->CreateString(stringValue, returnValue);
        }

        case JsonValueType_Object: {
            ComPtr<IJsonObject> objectValue;
            ASSERT_SUCCESS(value->GetObject(objectValue.GetAddressOf()));

            ComPtr<JsonPropertyProvider> provider(Make<JsonPropertyProvider>(objectValue.Get()));
            return provider.Get()->QueryInterface(IID_IInspectable, reinterpret_cast<void**>(returnValue));
        }

        case JsonValueType_Array: {
            ComPtr<IJsonArray> arrayValue;
            ASSERT_SUCCESS(value->GetArray(arrayValue.GetAddressOf()));

            ComPtr<JsonVectorView> provider(Make<JsonVectorView>(arrayValue.Get()));
            return provider.Get()->QueryInterface(IID_IInspectable, reinterpret_cast<void**>(returnValue));
        }
    }

    *returnValue = nullptr;
    return S_OK;
}

JsonProperty::JsonProperty(HSTRING name) {
    ASSERT_SUCCESS(_name.Set(name));
}

STDMETHODIMP JsonProperty::get_Type(TypeName* value) {
    *value = {nullptr, TypeKind_Custom};
    return S_OK;
}

STDMETHODIMP JsonProperty::get_Name(HSTRING* value) {
    return _name.CopyTo(value);
}

STDMETHODIMP JsonProperty::GetValue(IInspectable* target, IInspectable** returnValue) {
    ComPtr<ABI::CxxGameraww::IJsonPropertyProvider> jsonPropertyProvider;
    HString runtimeClassName;
    target->GetRuntimeClassName(runtimeClassName.GetAddressOf());

    ASSERT_SUCCESS(target->QueryInterface(ABI::CxxGameraww::IID_IJsonPropertyProvider, reinterpret_cast<void**>(jsonPropertyProvider.GetAddressOf())));

    ComPtr<IInspectable> objectInspectable;
    ASSERT_SUCCESS(jsonPropertyProvider->get_Object(&objectInspectable));

    ComPtr<IMap<HSTRING, IJsonValue*>> objectMap;
    ASSERT_SUCCESS(objectInspectable.As(&objectMap));

    boolean hasKey = false;
    ASSERT_SUCCESS(objectMap->HasKey(_name.Get(), &hasKey));

    if (hasKey) {
        ComPtr<IJsonValue> value;
        ASSERT_SUCCESS(objectMap->Lookup(_name.Get(), value.GetAddressOf()));
        return ValueToObject(value.Get(), returnValue);
    }

    *returnValue = nullptr;
    return S_OK;
}

STDMETHODIMP JsonProperty::SetValue(IInspectable* target, IInspectable* value) {
    return S_OK;
}

STDMETHODIMP JsonProperty::GetIndexedValue(IInspectable* target, IInspectable* index, IInspectable** returnValue) {
    *returnValue = nullptr;
    return S_OK;
}

STDMETHODIMP JsonProperty::SetIndexedValue(IInspectable* target, IInspectable* value, IInspectable* index) {
    return S_OK;
}

STDMETHODIMP JsonProperty::get_CanWrite(boolean* value) {
    *value = true;
    return S_OK;
}

STDMETHODIMP JsonProperty::get_CanRead(boolean* value) {
    *value = true;
    return S_OK;
}

};
