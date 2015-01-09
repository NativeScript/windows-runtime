#include "pch.h"
#include "App.h"

namespace NativeScript {
namespace TestRunner {

using namespace std;
using namespace Windows::UI::Popups;
using namespace Windows::ApplicationModel::Activation;
using namespace TestFixtures;

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    (ref new MessageDialog(L"Loaded"))->ShowAsync();
}

}
}
