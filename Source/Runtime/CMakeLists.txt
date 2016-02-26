set(HEADER_FILES
    Runtime-Prefix.h
    Runtime.h
    RuntimeExportMacros.h
    GlobalObject.h

    ReleasePool.h

    FFI/FFIBooleanType.h
    FFI/FFIInterop.h
    FFI/FFINumberType.h
    FFI/FFIType.h
    FFI/FFIVoidType.h

    COM/COMConstructor.h
    COM/COMInterop.h
    COM/COMMethodCall.h
    COM/COMObjectMap.h
    COM/COMObjectWrapper.h
    COM/COMUnknownType.h
    COM/HSTRINGType.h
)

set(SOURCE_FILES
    Runtime-Prefix.cpp
    Runtime.cpp
    GlobalObject.cpp

    FFI/FFIInterop.cpp
    FFI/FFIVoidType.cpp

    COM/COMConstructor.cpp
    COM/COMInterop.cpp
    COM/COMMethodCall.cpp
    COM/COMObjectWrapper.cpp
    COM/COMUnknownType.cpp
    COM/HSTRINGType.cpp
)

include_directories(${WEBKIT_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/Metadata ${PROJECT_SOURCE_DIR}/libffi)
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
