#include "pch.h"
#include "App.h"

using namespace Microsoft::WRL::Wrappers;
using namespace Microsoft::WRL;

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml;

using namespace CxxGameraww;

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int) {
    RoInitializeWrapper init{RO_INIT_MULTITHREADED};

    ComPtr<IApplicationStatics> applicationStatics;
    ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Application}.Get(), &applicationStatics));

    ComPtr<IApplicationInitializationCallback> callback{Callback<IApplicationInitializationCallback>([](IApplicationInitializationCallbackParams*) {
        ComPtr<IApplicationFactory> applicationFactory;
        ASSERT_SUCCESS(GetActivationFactory(HStringReference{RuntimeClass_Windows_UI_Xaml_Application}.Get(), &applicationFactory));

        ComPtr<App> outerApp{Make<App>()};
        IInspectable* innerApp;
        ComPtr<IApplication> app;
        ASSERT_SUCCESS(applicationFactory->CreateInstance(outerApp.Get(), &innerApp, app.GetAddressOf()));

        ComPtr<IApplicationOverrides> baseApp;
        ASSERT_SUCCESS(innerApp->QueryInterface(__uuidof(baseApp), &baseApp));
        outerApp->SetBase(baseApp.Get());

        return S_OK;
    })};

    ASSERT_SUCCESS(applicationStatics->Start(callback.Get()));
    return 0;
}
