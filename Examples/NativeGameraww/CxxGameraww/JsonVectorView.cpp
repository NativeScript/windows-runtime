#include "pch.h"

#include "JsonVectorView.h"
#include "JsonProperty.h"

namespace CxxGameraww {
using namespace Microsoft::WRL;
using namespace ABI::Windows::UI::Xaml::Data;
using namespace ABI::Windows::UI::Xaml::Interop;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::Data::Json;

JsonVectorView::JsonVectorView(IJsonArray* array) :
    _array(array) {
}

STDMETHODIMP JsonVectorView::GetAt(unsigned index, IInspectable** item) {
    ComPtr<IVector<IJsonValue*>> vector;
    ASSERT_SUCCESS(_array.As(&vector));

    ComPtr<IJsonValue> object;
    ASSERT_SUCCESS(vector->GetAt(index, object.GetAddressOf()));
    return ValueToObject(object.Get(), item);
}

STDMETHODIMP JsonVectorView::get_Size(unsigned* size) {
    ComPtr<IVector<IJsonValue*>> vector;
    ASSERT_SUCCESS(_array.As(&vector));

    return vector->get_Size(size);
}

STDMETHODIMP JsonVectorView::IndexOf(IInspectable* value, unsigned* index, boolean* found) {
    ComPtr<IVector<IInspectable*>> vector;
    ASSERT_SUCCESS(_array.As(&vector));

    return vector->IndexOf(vector.Get(), index, found);
}

STDMETHODIMP JsonVectorView::First(IIterator<IInspectable*>** first) {
    ComPtr<IIterable<IJsonValue*>> iterable;
    ASSERT_SUCCESS(_array.As(&iterable));

    ComPtr<IIterator<IJsonValue*>> iterator;
    ASSERT_SUCCESS(iterable->First(iterator.GetAddressOf()));

    *first = Make<JsonVectorViewIterator>(iterator.Get()).Detach();
    return S_OK;
}

JsonVectorViewIterator::JsonVectorViewIterator(IIterator<IJsonValue*>* jsonValueIterator)
    : _jsonValueIterator(jsonValueIterator) {
}

STDMETHODIMP JsonVectorViewIterator::get_Current(IInspectable** current) {
    ComPtr<IJsonValue> value;
    ASSERT_SUCCESS(_jsonValueIterator->get_Current(value.GetAddressOf()));

    return ValueToObject(value.Get(), current);
}

STDMETHODIMP JsonVectorViewIterator::get_HasCurrent(boolean* hasCurrent) {
    return _jsonValueIterator->get_HasCurrent(hasCurrent);
}

STDMETHODIMP JsonVectorViewIterator::MoveNext(boolean* hasCurrent) {
    return _jsonValueIterator->MoveNext(hasCurrent);
}

}
