#include "pch.h"
#include "App.h"
#include "Metadata.h"

namespace NativeScript {
namespace TestApp {

using namespace Tests::Fixtures;
using namespace Windows::UI::Popups;
using namespace Windows::ApplicationModel::Activation;

void App::OnLaunched(LaunchActivatedEventArgs^ e) {
    Metadata::MetadataReader metadataReader = Metadata::MetadataReader::instance();
    Class1 class1;

    (ref new MessageDialog(L"Loaded"))->ShowAsync();
}

}
}