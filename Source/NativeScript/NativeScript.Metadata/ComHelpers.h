#pragma once

namespace NativeScript {

#ifdef _DEBUG
#define ASSERT(booleanExpression)                                                            \
    do {                                                                                     \
        if (!(booleanExpression)) {                                                          \
            ::RaiseException(ERROR_ASSERTION_FAILURE, EXCEPTION_NONCONTINUABLE, 0, nullptr); \
        }                                                                                    \
    } while (0)
#else
#define ASSERT(booleanExpression)
#endif

#ifdef _DEBUG
#define ASSERT_HR(hr)                                                          \
    do {                                                                       \
        HRESULT __hresult{ hr };                                               \
        if (FAILED(__hresult)) {                                               \
            ::RaiseException(__hresult, EXCEPTION_NONCONTINUABLE, 0, nullptr); \
        }                                                                      \
    } while (0)
#else
#define ASSERT_HR(hr) ((void)hr)
#endif

#ifdef _DEBUG
void DEBUG_LOG(_Printf_format_string_ const wchar_t* format, ...);
#else
#define DEBUG_LOG(format, ...)
#endif
}