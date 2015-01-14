#include "Runtime.h"
#include <JavaScriptCore/JavaScript.h>
#include <ffi.h>
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

    {
        ffi_cif cif;
        ffi_type* args[1];
        void* values[1];
        char* s;
        int rc;

        /* Initialize the argument info vectors */
        args[0] = &ffi_type_pointer;
        values[0] = &s;

        /* Initialize the cif */
        if (ffi_prep_cif(&cif, FFI_DEFAULT_ABI, 1, &ffi_type_uint, args) == FFI_OK) {
            s = "Hello World!";
            ffi_call(&cif, FFI_FN(strlen), &rc, values);
            /* rc now holds the result of the call to strlen */

            /* values holds a pointer to the function's arg, so to
            call strlen() again all we need to do is change the
            value of s */
            s = "This is cool!";
            ffi_call(&cif, FFI_FN(strlen), &rc, values);
        }
    }
}

}
