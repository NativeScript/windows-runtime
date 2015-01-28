#pragma once

#include <Runtime.h>

namespace NativeScript {
namespace TestRunner {

    ref class App sealed : public Windows::UI::Xaml::Application {
    public:
        App();

protected:
    virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

private:
    Runtime _runtime;
};

}
}
