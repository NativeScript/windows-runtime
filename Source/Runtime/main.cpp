#include <JavaScriptCore/JavaScript.h>
#include <windows.h>

int main(int argc, char** argv) {
    JSGlobalContextRef ctx = JSGlobalContextCreate(nullptr);

    return 0;
}

#define DLL_EXPORT extern "C" __declspec(dllexport)

DLL_EXPORT
BOOL WINAPI DllMain(
    _In_  HINSTANCE hinstDLL,
    _In_  DWORD fdwReason,
    _In_  LPVOID lpvReserved
    ) {
    main(0, nullptr);
    return true;
}
