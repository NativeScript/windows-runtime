#pragma once

namespace NativeScript {
namespace TestRunner {

ref class App : public Windows::UI::Xaml::Application {
protected:
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
};

}
}
