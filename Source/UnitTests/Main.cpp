#include "UnitTests-Prefix.h"

using namespace Platform;
using namespace Windows::UI::Xaml;

ref class App sealed : public Windows::UI::Xaml::Application {
public:
    App()
        : Windows::UI::Xaml::Application() {

    }

protected:
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override {
        Microsoft::VisualStudio::TestPlatform::TestExecutor::WinRTCore::UnitTestClient::CreateDefaultUI();
        Window::Current->Activate();
        Microsoft::VisualStudio::TestPlatform::TestExecutor::WinRTCore::UnitTestClient::Run(e->Arguments);
    }
};

int main(Array<String^>^ args) {
    Application::Start(ref new ApplicationInitializationCallback([](ApplicationInitializationCallbackParams^ p) {
        App^ app(ref new App());
    }));
}