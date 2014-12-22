#include "pch.h"

#include <windows.ui.xaml.markup.h>
#include <windows.data.json.h>
#include <windows.system.threading.h>
#include <windows.ui.core.h>

#include "App.h"
#include "AgileCallback.h"
#include "JsonPropertyProvider.h"
#include "JsonProperty.h"
#include "JsonVectorView.h"

namespace CxxGameraww {
using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::ApplicationModel::Core;
using namespace ABI::Windows::ApplicationModel::Activation;
using namespace ABI::Windows::Web::Http;
using namespace ABI::Windows::Data::Json;
using namespace ABI::Windows::System::Threading;
using namespace ABI::Windows::UI::Core;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::Xaml::Media;
using namespace ABI::Windows::UI::Xaml::Markup;
using namespace ABI::Windows::UI::Xaml;

const wchar_t* MAINPAGE_XAML{LR"(
<Page
    xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
    xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
    Background="{ThemeResource ApplicationPageBackgroundThemeBrush}">
    <Grid Margin="10,0,10,0">
        <Grid.RowDefinitions>
            <RowDefinition Height="Auto" />
            <RowDefinition Height="*" />
        </Grid.RowDefinitions>
        <TextBlock Grid.Row="0" Text="Gameraww" Style="{ThemeResource HeaderTextBlockStyle}" Margin="0,0,0,10" />
        <ListView Grid.Row="1" ItemsSource="{Binding data.children}">
            <ListView.ItemTemplate>
                <DataTemplate>
                    <Grid Margin="0,0,0,10">
                        <Grid.ColumnDefinitions>
                            <ColumnDefinition Width="Auto"/>
                            <ColumnDefinition Width="*"/>
                        </Grid.ColumnDefinitions>
                        <Image Grid.Column="0" Width="100" Height="100" Margin="0,0,10,0" Source="{Binding data.thumbnail}" Stretch="UniformToFill" />
                        <StackPanel Grid.Column="1" Background="{StaticResource ApplicationPageBackgroundThemeBrush}">
                            <TextBlock Text="{Binding data.title}" Style="{ThemeResource BaseTextBlockStyle}" />
                        </StackPanel>
                    </Grid>
                </DataTemplate>
            </ListView.ItemTemplate>
        </ListView>
    </Grid>
</Page>
)"};

STDMETHODIMP App::OnActivated(IActivatedEventArgs* args) {
    return _base->OnActivated(args);
}

STDMETHODIMP App::OnLaunched(ILaunchActivatedEventArgs* args) {
    ComPtr<IWorkItemHandler> threadHandler = Callback<IWorkItemHandler>([](IAsyncAction*) -> HRESULT {
        ComPtr<IUriRuntimeClassFactory> uriStatics;
        ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_Foundation_Uri}.Get(), &uriStatics));
        ComPtr<IUriRuntimeClass> uri;
        ASSERT_SUCCESS(uriStatics->CreateUri(HStringReference{L"http://www.reddit.com/r/aww.json?limit=50"}.Get(), uri.GetAddressOf()));

        ComPtr<IActivationFactory> httpClientStatics;
        ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_Web_Http_HttpClient}.Get(), &httpClientStatics));
        ComPtr<IInspectable> httpClientInspectable;
        ASSERT_SUCCESS(httpClientStatics->ActivateInstance(httpClientInspectable.GetAddressOf()));
        ComPtr<IHttpClient> httpClient;
        ASSERT_SUCCESS(httpClientInspectable.As(&httpClient));

        ComPtr<IAsyncOperationWithProgress<HSTRING, HttpProgress>> downloadOperation;
        ASSERT_SUCCESS(httpClient->GetStringAsync(uri.Get(), downloadOperation.GetAddressOf()));

        ComPtr<IAsyncOperationWithProgressCompletedHandler<HSTRING, HttpProgress>> downloadHandler = Callback<IAsyncOperationWithProgressCompletedHandler<HSTRING, HttpProgress>>([httpClient](IAsyncOperationWithProgress<HSTRING, HttpProgress>* downloadOperationCb, AsyncStatus status) -> HRESULT {
            HString jsonContents;
            ASSERT_SUCCESS(downloadOperationCb->GetResults(jsonContents.GetAddressOf()));

            ComPtr<IAsyncInfo> downloadOperationIAsyncInfo;
            ASSERT_SUCCESS(downloadOperationCb->QueryInterface(__uuidof(downloadOperationIAsyncInfo), &downloadOperationIAsyncInfo));
            ASSERT_SUCCESS(downloadOperationIAsyncInfo->Close());

            ComPtr<IClosable> httpClientClosable;
            ASSERT_SUCCESS(httpClient.As(&httpClientClosable));
            ASSERT_SUCCESS(httpClientClosable->Close());

            ComPtr<IJsonObjectStatics> jsonObjectStatics;
            ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_Data_Json_JsonObject).Get(), &jsonObjectStatics));

            ComPtr<IJsonObject> json;
            ASSERT_SUCCESS(jsonObjectStatics->Parse(jsonContents.Get(), json.GetAddressOf()));

            ComPtr<ICoreImmersiveApplication> coreApplicationStatics;
            ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_ApplicationModel_Core_CoreApplication).Get(), &coreApplicationStatics));
            ComPtr<ICoreApplicationView> mainView;
            ASSERT_SUCCESS(coreApplicationStatics->get_MainView(mainView.GetAddressOf()));
            ComPtr<ICoreWindow> coreWindow;
            ASSERT_SUCCESS(mainView->get_CoreWindow(coreWindow.GetAddressOf()));
            ComPtr<ICoreDispatcher> coreDispatcher;
            ASSERT_SUCCESS(coreWindow->get_Dispatcher(coreDispatcher.GetAddressOf()));

            ComPtr<IDispatchedHandler> dispatchHandler = AgileCallback<IDispatchedHandler>([json]() -> HRESULT {
                ComPtr<IXamlReaderStatics> xamlReaderStatics;
                ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Markup_XamlReader).Get(), &xamlReaderStatics));
                ComPtr<IInspectable> mainPageXamlInspectable;
                ASSERT_SUCCESS(xamlReaderStatics->Load(HStringReference(MAINPAGE_XAML).Get(), &mainPageXamlInspectable));

                ComPtr<IFrameworkElement> mainPageXamlFrameworkElement;
                ASSERT_SUCCESS(mainPageXamlInspectable.As(&mainPageXamlFrameworkElement));
                ComPtr<JsonPropertyProvider> jsonPropertyProvider = Make<JsonPropertyProvider>(json.Get());
                ComPtr<IInspectable> jsonPropertyProviderInspectable;
                ASSERT_SUCCESS(jsonPropertyProvider.As(&jsonPropertyProviderInspectable));
                ASSERT_SUCCESS(mainPageXamlFrameworkElement->put_DataContext(jsonPropertyProviderInspectable.Get()));

                ComPtr<IWindowStatics> windowStatics;
                ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Window}.Get(), &windowStatics));
                ComPtr<IWindow> currentWindow;
                ASSERT_SUCCESS(windowStatics->get_Current(currentWindow.GetAddressOf()));

                ComPtr<IUIElement> content;
                ASSERT_SUCCESS(mainPageXamlInspectable.As(&content));
                ASSERT_SUCCESS(currentWindow->put_Content(content.Get()));

                ASSERT_SUCCESS(currentWindow->Activate());

                return S_OK;
            });
            ComPtr<IAsyncAction> dispatcherAsync;
            IDispatchedHandler* iDispatchedHandler{dispatchHandler.Get()};
            ASSERT_SUCCESS(coreDispatcher->RunAsync(CoreDispatcherPriority_Normal, iDispatchedHandler, dispatcherAsync.GetAddressOf()));

            return S_OK;
        });
        ASSERT_SUCCESS(downloadOperation->put_Completed(downloadHandler.Get()));
        return S_OK;
    });

    ComPtr<IThreadPoolStatics> threadPoolStatics;
    ASSERT_SUCCESS(GetActivationFactory(HStringReference(RuntimeClass_Windows_System_Threading_ThreadPool).Get(), &threadPoolStatics));
    ComPtr<IAsyncAction> asyncOperation;
    ASSERT_SUCCESS(threadPoolStatics->RunAsync(threadHandler.Get(), asyncOperation.GetAddressOf()));

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
