#include "pch.h"
#include "ComHelpers.h"
#include <array>

namespace NativeScript {

#ifdef _DEBUG
void DEBUG_LOG(_Printf_format_string_ const wchar_t* format, ...) {
    va_list args;
    va_start(args, format);
    std::array<wchar_t, 4096> buffer;
    _vsnwprintf_s(buffer.data(), buffer.size(), _TRUNCATE, format, args);
    va_end(args);

    OutputDebugString(buffer.data());
}
#endif

}