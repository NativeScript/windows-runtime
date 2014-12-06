#pragma once

// TODO: Move and refactor

#include <array>

#define NO_RETURN __declspec(noreturn)
NO_RETURN void CRASH();

namespace NativeScript {
namespace Metadata {

// This limit is only for C# and not for the CLI, but it is good enough
const size_t MAX_IDENTIFIER_LENGTH{511};
using identifier = std::array<wchar_t, MAX_IDENTIFIER_LENGTH + 1>;

#ifdef _DEBUG
#define ASSERT(booleanExpression)                                                            \
    do {                                                                                     \
        if (!(booleanExpression)) {                                                          \
            ::RaiseException(ERROR_ASSERTION_FAILURE, EXCEPTION_NONCONTINUABLE, 0, nullptr); \
            ::CRASH();                                                                       \
        }                                                                                    \
    } while (0)
#else
#define ASSERT(booleanExpression)
#endif

#ifdef _DEBUG
#define ASSERT_NOT_REACHED() CRASH()
#else
#define ASSERT_NOT_REACHED()
#endif

#ifdef _DEBUG
#define ASSERT_SUCCESS(hr)                                                     \
    do {                                                                       \
        HRESULT __hresult{ hr };                                               \
        if (FAILED(__hresult)) {                                               \
            ::RaiseException(__hresult, EXCEPTION_NONCONTINUABLE, 0, nullptr); \
            ::CRASH();                                                         \
        }                                                                      \
    } while (0)
#else
#define ASSERT_SUCCESS(hr) ((void)hr)
#endif

#ifdef _DEBUG
void DEBUG_LOG(_Printf_format_string_ const wchar_t* format, ...);
#else
#define DEBUG_LOG(format, ...)
#endif

std::wstring getUnaryCustomAttributeStringValue(IMetaDataImport2*, mdToken, const wchar_t* attributeName);

GUID getGuidAttributeValue(IMetaDataImport2*, mdToken);

}
}
