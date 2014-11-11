#include "pch.h"
#include "App.h"

using namespace Platform;
using namespace Windows::UI::Xaml;

int __cdecl main(Array<String^>^ args) {
    Application::Start(ref new ApplicationInitializationCallback([](ApplicationInitializationCallbackParams^ p) {
        ref new NativeScript::TestApp::App();
    }));
}