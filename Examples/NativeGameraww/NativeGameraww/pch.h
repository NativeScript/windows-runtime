#pragma once

#include <wrl.h>
#include <windows.ui.xaml.h>

#ifdef _DEBUG
#define ASSERT_SUCCESS(hr)                                                     \
    do {                                                                       \
        HRESULT __hresult{hr};                                                 \
        if (FAILED(__hresult)) {                                               \
            ::RaiseException(__hresult, EXCEPTION_NONCONTINUABLE, 0, nullptr); \
        }                                                                      \
    } while (0)
#else
#define ASSERT_SUCCESS(hr) ((void)hr)
#endif
