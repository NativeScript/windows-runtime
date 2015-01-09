set(HEADER_FILES
    RuntimeExportMacros.h
    pch.h
)

set(SOURCE_FILES
    main.cpp
)

source_group("" FILES ${SOURCE_FILES} ${HEADER_FILES})

include_directories(${WEBKIT_INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/Metadata)
link_directories(${WEBKIT_LINK_DIRECTORIES})

add_library(Runtime SHARED ${SOURCE_FILES} ${HEADER_FILES})
add_dependencies(Runtime WebKit)
target_link_libraries(Runtime JavaScriptCore Metadata)

include(GenerateExportHeader)
generate_export_header(Runtime
    BASE_NAME TNS
    EXPORT_FILE_NAME RuntimeExportMacros.h
)
