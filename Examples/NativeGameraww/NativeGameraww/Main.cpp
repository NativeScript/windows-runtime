#include "pch.h"
#include "App.h"

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml;

using namespace NativeGameraww;

static STDMETHODIMP initApplication(IApplicationInitializationCallbackParams*) {
    ComPtr<IApplicationFactory> applicationStatics;
    ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Application}.Get(), &applicationStatics));

    ComPtr<App> outerApp{Make<App>()};
    IInspectable* innerApp;
    ComPtr<IApplication> app;
    ASSERT_SUCCESS(applicationStatics->CreateInstance(outerApp.Get(), &innerApp, app.GetAddressOf()));

    ComPtr<IApplicationOverrides> baseApp;
    ASSERT_SUCCESS(innerApp->QueryInterface(__uuidof(baseApp), &baseApp));
    outerApp->SetBase(baseApp.Get());

    return S_OK;
}

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    RoInitializeWrapper init{RO_INIT_MULTITHREADED};

    ComPtr<IApplicationStatics> applicationStatics;
    ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Application}.Get(), &applicationStatics));

    ComPtr<IApplicationInitializationCallback> callback{Callback<IApplicationInitializationCallback>(initApplication)};
    ASSERT_SUCCESS(applicationStatics->Start(callback.Get()));

    return 0;
}
