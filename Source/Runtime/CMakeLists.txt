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
    COM/COMObjectMap.h
    COM/COMInterop.h
)

set(SOURCE_FILES
    Runtime-Prefix.cpp
    Runtime.cpp
    GlobalObject.cpp
    
    FFI/FFIVoidType.cpp
    FFI/FFIInterop.cpp

    COM/COMObjectWrapper.cpp
    COM/COMInterop.cpp
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
