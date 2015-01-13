#include "pch.h"
#include "App.h"
#include <Runtime.h>

namespace NativeScript {
namespace TestRunner {

using namespace std;
using namespace Windows::UI::Popups;
using namespace Windows::ApplicationModel::Activation;
using namespace TestFixtures;

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    Runtime runtime;

    (ref new MessageDialog(L"Loaded"))->ShowAsync();
}

}
}
