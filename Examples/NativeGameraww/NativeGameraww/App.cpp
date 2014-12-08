#include "pch.h"
#include "App.h"

namespace NativeGameraww {
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel::Activation;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Xaml;

STDMETHODIMP App::OnActivated(IActivatedEventArgs* args) {
    return _base->OnActivated(args);
}

STDMETHODIMP App::OnLaunched(ILaunchActivatedEventArgs* args) {
    ComPtr<IWindowStatics> windowStatics;
    ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Window}.Get(), &windowStatics));

    ComPtr<IWindow> currentWindow;
    ASSERT_SUCCESS(windowStatics->get_Current(currentWindow.GetAddressOf()));

    ComPtr<ITextBoxFactory> textBoxStatics;
    ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Controls_TextBox}.Get(), &textBoxStatics));

    ComPtr<ITextBox> textBox;
    ASSERT_SUCCESS(textBoxStatics->CreateInstance(nullptr, nullptr, textBox.GetAddressOf()));
    ASSERT_SUCCESS(textBox->put_Text(HStringReference{L"Hello, World!"}.Get()));

    ComPtr<IUIElement> content;
    ASSERT_SUCCESS(textBox.As(&content));
    ASSERT_SUCCESS(currentWindow->put_Content(content.Get()));

    ASSERT_SUCCESS(currentWindow->Activate());
    return S_OK;
}

STDMETHODIMP App::OnFileActivated(IFileActivatedEventArgs* args) {
    return _base->OnFileActivated(args);
}

STDMETHODIMP App::OnSearchActivated(ISearchActivatedEventArgs* args) {
    return _base->OnSearchActivated(args);
}

STDMETHODIMP App::OnShareTargetActivated(IShareTargetActivatedEventArgs* args) {
    return _base->OnShareTargetActivated(args);
}

STDMETHODIMP App::OnFileOpenPickerActivated(IFileOpenPickerActivatedEventArgs* args) {
    return _base->OnFileOpenPickerActivated(args);
}

STDMETHODIMP App::OnFileSavePickerActivated(IFileSavePickerActivatedEventArgs* args) {
    return _base->OnFileSavePickerActivated(args);
}

STDMETHODIMP App::OnCachedFileUpdaterActivated(ICachedFileUpdaterActivatedEventArgs* args) {
    return _base->OnCachedFileUpdaterActivated(args);
}

STDMETHODIMP App::OnWindowCreated(IWindowCreatedEventArgs* args) {
    return _base->OnWindowCreated(args);
}

}
