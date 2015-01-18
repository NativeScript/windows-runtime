#include "pch.h"
#include "App.h"
#include <JavaScriptCore/JavaScript.h>
#include <Runtime.h>
#include <sstream>

namespace NativeScript {
namespace TestRunner {

using namespace std;
using namespace Windows::UI::Popups;
using namespace Windows::ApplicationModel::Activation;
using namespace TestFixtures;

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    Runtime runtime(Windows::ApplicationModel::Package::Current->InstalledLocation->Path->Data());

    JSStringRef script = JSStringCreateWithUTF8CString("6 * 7");
    JSValueRef value = JSEvaluateScript(runtime.globalContext, script, nullptr, nullptr, 0, nullptr);
    JSStringRelease(script);
    double result = JSValueToNumber(runtime.globalContext, value, nullptr);

    std::wostringstream text;
    text << L"The result of JavaScript calculation is " << result;
    (ref new MessageDialog(Platform::StringReference(text.str().c_str())))->ShowAsync();
}

}
}
