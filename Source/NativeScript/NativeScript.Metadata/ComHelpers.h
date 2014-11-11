#pragma once

namespace NativeScript {

#ifdef _DEBUG
void ASSERT_HR(HRESULT hr);
#else
#define ASSERT_HR(hr) ((void)hr)
#endif

#ifdef _DEBUG
void DEBUG_LOG(const wchar_t* format, ...); // TODO: Use SAL Annotation
#else
#define DEBUG_LOG(format, ...)
#endif

}