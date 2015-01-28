#pragma once

#include <ffi.h>
#include <JavaScriptCore/ClassInfo.h>

namespace NativeScript {
    struct FFITypeMethodTable {
        JSC::JSValue (*marshalNativeToJS)(const JSC::JSCell*, JSC::ExecState*, const void*);

        void (*marshalJSToNative)(const JSC::JSCell*, JSC::ExecState*, JSC::JSValue, void*);

        const ffi_type* (*getFFITypeStruct)(const JSC::JSCell*);
    };

#define CREATE_FFI_METHOD_TABLE(ClassName) { \
        &ClassName::marshalNativeToJS, \
        &ClassName::marshalJSToNative, \
        &ClassName::getFFITypeStruct,  \
    }

    struct FFITypeClassInfo : JSC::ClassInfo {
        FFITypeMethodTable ffiMethodTable;

        FFITypeClassInfo() { }

        FFITypeClassInfo(const char* className, const JSC::ClassInfo* parentClass, const JSC::HashTable* staticPropHashTable, JSC::MethodTable methodTable, JSC::TypedArrayType typedArrayStorageType, FFITypeMethodTable ffiMethodTable)
        {
            this->className = className;
            this->parentClass = parentClass;
            this->staticPropHashTable = staticPropHashTable;
            this->methodTable = methodTable;
            this->typedArrayStorageType = typedArrayStorageType;
            this->ffiMethodTable = ffiMethodTable;
        }
    };

#define DECLARE_FFI_INFO                                                                \
    protected:                                                                          \
        static const ::NativeScript::FFITypeClassInfo s_info;                           \
    public:                                                                             \
        static const ::NativeScript::FFITypeClassInfo* info() { return &s_info; }

    static const unsigned IsFFITypeFlag = 1 << 15;

    inline bool isFFIType(JSC::JSCell* cell) {
        if (!cell) {
            return false;
        }

        return cell->structure()->typeInfo().flags() & IsFFITypeFlag;
    }

    inline const FFITypeMethodTable* getFFIMethodTable(JSC::JSCell* cell) {
        ASSERT(isFFIType(cell));

        const FFITypeClassInfo* classInfo = static_cast<const FFITypeClassInfo*>(cell->classInfo());
        return &classInfo->ffiMethodTable;
    }
}