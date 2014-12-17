#include "pch.h"
#include "App.h"

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace NativeScript::TestApp;

int __cdecl main(Array<String^>^ args) {
    Application::Start(ref new ApplicationInitializationCallback([](ApplicationInitializationCallbackParams^ p) {
        App^ app(ref new App());

#ifdef _DEBUG
        app->UnhandledException += ref new UnhandledExceptionEventHandler([](Object^ sender, UnhandledExceptionEventArgs^ e) {
            if (IsDebuggerPresent()) {
                String^ errorMessage(e->Message);
                __debugbreak();
            }
        });
#endif
    }));
}
