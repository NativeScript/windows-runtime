#pragma once

#include "RuntimeExportMacros.h"

#ifndef JSGlobalContextRef

// forward declare the JavaScriptCore types we'll expose
typedef struct OpaqueJSContext* JSGlobalContextRef;
typedef const struct OpaqueJSValue* JSValueRef;

#endif

namespace NativeScript {

typedef struct RuntimeImpl* RuntimeImplRef;

class TNS_EXPORT Runtime final {
public:
    const std::wstring applicationPath;
    const JSGlobalContextRef globalContext;

    Runtime(std::wstring applicationPath);
    ~Runtime();

    // Always call this on the main thread,
    // before creating any instances of this class.
    static void initialize();

    void executeModule(const wchar_t* moduleIdentifier, JSValueRef** error);
private:
    Runtime(RuntimeImplRef, std::wstring);

    const RuntimeImplRef _impl;
};

}
