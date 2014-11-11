#include "pch.h"
#include "ComHelpers.h"

namespace NativeScript {

#ifdef _DEBUG
void ASSERT_HR(HRESULT hr) {
    if (FAILED(hr)) {
        _com_error com_error(hr, nullptr);
        DEBUG_LOG(L"Error: %s\n", com_error.ErrorMessage());
        RaiseException(hr, EXCEPTION_NONCONTINUABLE, 0, nullptr);
    }
}
#endif

#ifdef _DEBUG
void DEBUG_LOG(const wchar_t* format, ...) {
    va_list args;
    va_start(args, format);
    wchar_t buffer[4096];
    _vsnwprintf_s(buffer, sizeof(buffer), format, args);
    va_end(args);

    OutputDebugString(buffer);
}
#endif

}