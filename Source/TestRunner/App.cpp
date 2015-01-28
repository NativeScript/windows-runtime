#include "TestRunner-Prefix.h"
#include "App.h"
#include <JavaScriptCore/JavaScript.h>
#include <sstream>
#include <ppltasks.h>

namespace NativeScript {
namespace TestRunner {

using namespace std;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Storage;
using namespace Windows::UI::Popups;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace TestFixtures;

App::App()
    : Application()
    , _runtime(Package::Current->InstalledLocation->Path->Data()) {
}

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    Uri^ uri = ref new Uri("ms-appx:///app/Test.js");

    create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([this](task<StorageFile ^ > fileTask) {
        create_task(FileIO::ReadTextAsync(fileTask.get())).then([this](task<String^> stringTask) {
            String^ fileContents = stringTask.get();
            JSStringRef scriptSource = JSStringCreateWithCharacters(fileContents->Data(), fileContents->Length());
            JSEvaluateScript(this->_runtime.globalContext, scriptSource, nullptr, nullptr, 0, nullptr);
            JSStringRelease(scriptSource);
        });
    });
}

}
}
