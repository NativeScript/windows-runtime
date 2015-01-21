#include "TestRunner-Prefix.h"
#include "App.h"
#include <JavaScriptCore/JavaScript.h>
#include <Runtime.h>
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

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    Uri^ uri = ref new Uri("ms-appx:///app/Test.js");

    create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([](task<StorageFile^> fileTask) {
        create_task(FileIO::ReadTextAsync(fileTask.get())).then([](task<String^> stringTask) {
            Runtime runtime(Package::Current->InstalledLocation->Path->Data());

            String^ fileContents = stringTask.get();
            JSStringRef scriptSource = JSStringCreateWithCharacters(fileContents->Data(), fileContents->Length());
            JSEvaluateScript(runtime.globalContext, scriptSource, nullptr, nullptr, 0, nullptr);
            JSStringRelease(scriptSource);
        });
    });
}

}
}
