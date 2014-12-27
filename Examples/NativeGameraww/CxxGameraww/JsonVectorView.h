#pragma once

#include <wrl.h>
#include <windows.data.json.h>

namespace CxxGameraww {

class JsonVectorView : public Microsoft::WRL::RuntimeClass<
        ABI::Windows::Foundation::Collections::IVectorView<IInspectable*>,
        ABI::Windows::Foundation::Collections::IIterable<IInspectable*>
    > {
    InspectableClass(ABI::Windows::Foundation::Collections::IVectorView<IInspectable*>::z_get_rc_name_impl(), BaseTrust);

public:
    class Iterator : public Microsoft::WRL::RuntimeClass<ABI::Windows::Foundation::Collections::IIterator<IInspectable*>> {
        InspectableClass(ABI::Windows::Foundation::Collections::IIterator<IInspectable*>::z_get_rc_name_impl(), BaseTrust);

    public:
        explicit Iterator(IIterator<ABI::Windows::Data::Json::IJsonValue*>* jsonValueIterator);

        virtual HRESULT STDMETHODCALLTYPE get_Current(_Out_ IInspectable** current) override;

        virtual HRESULT STDMETHODCALLTYPE get_HasCurrent(_Out_ boolean* hasCurrent) override;

        virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ boolean* hasCurrent) override;

    private:
        Microsoft::WRL::ComPtr<IIterator<ABI::Windows::Data::Json::IJsonValue*>> _jsonValueIterator;
    };

    explicit JsonVectorView(ABI::Windows::Data::Json::IJsonArray*);

    virtual HRESULT STDMETHODCALLTYPE GetAt(_In_ unsigned index, _Out_ IInspectable** item) override;

    virtual /* propget */ HRESULT STDMETHODCALLTYPE get_Size(_Out_ unsigned* size) override;

    virtual HRESULT STDMETHODCALLTYPE IndexOf(_In_opt_ IInspectable* value, _Out_ unsigned* index, _Out_ boolean* found) override;

    virtual HRESULT STDMETHODCALLTYPE First(_Outptr_result_maybenull_ ABI::Windows::Foundation::Collections::IIterator<IInspectable*>** first) override;

private:
    Microsoft::WRL::ComPtr<ABI::Windows::Data::Json::IJsonArray> _array;
};

}
