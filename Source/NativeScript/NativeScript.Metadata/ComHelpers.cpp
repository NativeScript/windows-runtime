#include "pch.h"
#include "ComHelpers.h"

NO_RETURN void CRASH() {
    reinterpret_cast<void(*)()>(0xDEADDEAD)();
}

namespace NativeScript {
namespace Metadata {

using namespace std;

#ifdef _DEBUG
void DEBUG_LOG(_Printf_format_string_ const wchar_t* format, ...) {
    va_list args;
    va_start(args, format);
    array<wchar_t, 4096> buffer;
    _vsnwprintf_s(buffer.data(), buffer.size(), _TRUNCATE, format, args);
    va_end(args);

    OutputDebugString((wstring{buffer.data()} + L'\n').data());
}
#endif

}
}
