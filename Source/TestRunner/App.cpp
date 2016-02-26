#include "TestRunner-Prefix.h"
#include "App.h"
#include <JavaScriptCore/JavaScript.h>
#include <sstream>
#include <ppltasks.h>

namespace NativeScript {
namespace TestRunner {

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
    Uri^ uri = ref new Uri("ms-appx:///index.js");

    create_task(StorageFile::GetFileFromApplicationUriAsync(uri)).then([this](task<StorageFile ^ > fileTask) {
        create_task(FileIO::ReadTextAsync(fileTask.get())).then([this](task<String^> stringTask) {
            JSGlobalContextRef context = this->_runtime.globalContext;

            String^ fileContents = stringTask.get();
            JSStringRef scriptSource = JSStringCreateWithCharacters(fileContents->Data(), fileContents->Length());
            JSValueRef exception = nullptr;
            JSEvaluateScript(context, scriptSource, nullptr, nullptr, 0, &exception);
            JSStringRelease(scriptSource);

            if (exception) {
                JSStringRef exceptionMessage = JSValueToStringCopy(context, exception, NULL);
                size_t maxLength = JSStringGetMaximumUTF8CStringSize(exceptionMessage);
                auto exceptionMessageBuffer = std::make_unique<char[]>(maxLength);
                JSStringGetUTF8CString(exceptionMessage, exceptionMessageBuffer.get(), maxLength);
                JSStringRelease(exceptionMessage);

                OutputDebugStringA(exceptionMessageBuffer.get());
            }
        });
    });
}

}
}
