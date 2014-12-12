#pragma once

#include <wrl.h>
#include <windows.ui.xaml.h>

namespace CxxGameraww {

class App : public Microsoft::WRL::RuntimeClass<ABI::Windows::UI::Xaml::IApplicationOverrides> {
    InspectableClass(L"CxxGameraww.App", BaseTrust);

public:
    STDMETHOD(OnActivated)(ABI::Windows::ApplicationModel::Activation::IActivatedEventArgs*) override;

    STDMETHOD(OnLaunched)(ABI::Windows::ApplicationModel::Activation::ILaunchActivatedEventArgs*) override;

    STDMETHOD(OnFileActivated)(ABI::Windows::ApplicationModel::Activation::IFileActivatedEventArgs*) override;

    STDMETHOD(OnSearchActivated)(ABI::Windows::ApplicationModel::Activation::ISearchActivatedEventArgs*) override;

    STDMETHOD(OnShareTargetActivated)(ABI::Windows::ApplicationModel::Activation::IShareTargetActivatedEventArgs*) override;

    STDMETHOD(OnFileOpenPickerActivated)(ABI::Windows::ApplicationModel::Activation::IFileOpenPickerActivatedEventArgs*) override;

    STDMETHOD(OnFileSavePickerActivated)(ABI::Windows::ApplicationModel::Activation::IFileSavePickerActivatedEventArgs*) override;

    STDMETHOD(OnCachedFileUpdaterActivated)(ABI::Windows::ApplicationModel::Activation::ICachedFileUpdaterActivatedEventArgs*) override;

    STDMETHOD(OnWindowCreated)(ABI::Windows::UI::Xaml::IWindowCreatedEventArgs*) override;

    void App::SetBase(IApplicationOverrides* base) {
        _base = base;
    }

private:
    Microsoft::WRL::ComPtr<IApplicationOverrides> _base;
};

}
