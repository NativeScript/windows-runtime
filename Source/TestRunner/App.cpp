#include "pch.h"
#include "App.h"
#include "MetadataReader.h"

namespace NativeScript {
namespace TestRunner {

using namespace std;
using namespace Windows::UI::Popups;
using namespace Windows::ApplicationModel::Activation;
using namespace Metadata;
using namespace TestFixtures;

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    MetadataReader metadataReader;

    (ref new MessageDialog(L"Loaded"))->ShowAsync();
}

}
}
