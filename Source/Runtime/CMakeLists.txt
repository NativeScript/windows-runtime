set(HEADER_FILES
    Runtime-Prefix.h
    Runtime.h
    RuntimeExportMacros.h
    GlobalObject.h
    
    ReleasePool.h

    FFI/FFIType.h
    FFI/FFIVoidType.h
    FFI/FFINumberType.h
    FFI/FFIBooleanType.h
    FFI/FFIInterop.h

    COM/COMObjectWrapper.h
    COM/COMConstructor.h
    COM/COMObjectMap.h
    COM/COMUnknownType.h
    COM/HSTRINGType.h
    COM/COMInterop.h
    COM/COMMethodCall.h
)

set(SOURCE_FILES
    Runtime-Prefix.cpp
    Runtime.cpp
    GlobalObject.cpp
    
    FFI/FFIVoidType.cpp
    FFI/FFIInterop.cpp

    COM/COMObjectWrapper.cpp
    COM/COMConstructor.cpp
    COM/COMUnknownType.cpp
    COM/HSTRINGType.cpp
    COM/COMInterop.cpp
    COM/COMMethodCall.cpp
)

include_directories(${WEBKIT_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/Metadata ${PROJECT_SOURCE_DIR}/libffi/libffi.Shared)
link_directories(${WEBKIT_LINK_DIRECTORIES})

add_library(Runtime SHARED ${SOURCE_FILES} ${HEADER_FILES})
add_dependencies(Runtime WebKit libffi)

target_link_libraries(Runtime ${WEBKIT_LIBRARIES} Metadata)

include(GroupSources)
GroupSources(Runtime)

include(PrecompiledHeaders)
SET_PRECOMPILED_HEADER(Runtime Runtime-Prefix.h Runtime-Prefix.cpp)

include(GenerateExportHeader)
generate_export_header(Runtime
    BASE_NAME TNS
    EXPORT_FILE_NAME RuntimeExportMacros.h
)
