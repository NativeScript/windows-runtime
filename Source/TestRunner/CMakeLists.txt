set(HEADER_FILES
    App.h
    TestRunner-Prefix.h
)
set(SOURCE_FILES
    App.cpp
    Main.cpp
    TestRunner-Prefix.cpp
)

source_group("" FILES ${SOURCE_FILES} ${HEADER_FILES})

set(SHORT_NAME TestRunner)
set(PACKAGE_GUID 42B59DDB-1F83-4D8B-8945-88D76CC0B7D5)
set(APPLICATION_ID "App")
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/Package.appxmanifest.in
    ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
    @ONLY
)
list(APPEND RESOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest)

list(APPEND RESOURCE_FILES app/index.js)

file(GLOB ASSETS_FILES ${PROJECT_SOURCE_DIR}/cmake/Assets/*)
source_group("Assets" FILES ${ASSETS_FILES})
list(APPEND RESOURCE_FILES ${ASSETS_FILES})

set(PACKAGED_BINARIES ${WEBKIT_BINARIES})
set_source_files_properties(${PACKAGED_BINARIES} PROPERTIES GENERATED TRUE)
source_group("Binaries" FILES ${PACKAGED_BINARIES})
list(APPEND RESOURCE_FILES ${PACKAGED_BINARIES})

set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

include_directories(${PROJECT_SOURCE_DIR}/Runtime ${PROJECT_BINARY_DIR}/Runtime ${WEBKIT_INCLUDE_DIRECTORIES})
link_directories(${WEBKIT_LINK_DIRECTORIES})

add_executable(TestRunner WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(TestRunner Runtime)
add_dependencies(TestRunner TestFixtures)

include(PrecompiledHeaders)
SET_PRECOMPILED_HEADER(TestRunner TestRunner-Prefix.h TestRunner-Prefix.cpp)

set_target_properties(TestRunner PROPERTIES
    VS_WINRT_COMPONENT TRUE
)
