#include "Runtime.h"
#include <JavaScriptCore/JavaScript.h>
#include <thread>
#include <array>

namespace NativeScript {

Runtime::Runtime() {
    std::thread([] {
        JSGlobalContextRef ctx = JSGlobalContextCreate(nullptr);

        auto str = JSStringCreateWithUTF8CString("(function f(a, b){ return a + b; })");
        JSValueRef func = JSEvaluateScript(ctx, str, nullptr, nullptr, 0, nullptr);
        JSStringRelease(str);

        std::array<JSValueRef, 2> args{{
            JSValueMakeNumber(ctx, 2),
            JSValueMakeNumber(ctx, 3),
        }};
        JSValueRef result = JSObjectCallAsFunction(ctx, const_cast<JSObjectRef>(func), nullptr, args.size(), args.data(), nullptr);
        double intResult{JSValueToNumber(ctx, result, nullptr)};

        JSGlobalContextRelease(ctx);
    }).join();
}

}
