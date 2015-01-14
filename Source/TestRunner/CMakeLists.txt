set(HEADER_FILES
    pch.h
    App.h
)
set(SOURCE_FILES
    App.cpp
    Main.cpp
)
source_group("" FILES ${SOURCE_FILES} ${HEADER_FILES})

set(SHORT_NAME TestRunner)
set(PACKAGE_GUID 42B59DDB-1F83-4D8B-8945-88D76CC0B7D5)
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/Package_${CMAKE_SYSTEM_NAME}.appxmanifest.in
    ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
    @ONLY
)
list(APPEND RESOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest)

file(GLOB ASSETS_FILES ${PROJECT_SOURCE_DIR}/cmake/Assets_${CMAKE_SYSTEM_NAME}/*)
source_group("Assets" FILES ${ASSETS_FILES})
list(APPEND RESOURCE_FILES ${ASSETS_FILES})

set(PACKAGED_BINARIES ${WEBKIT_BINARIES})
set_source_files_properties(${PACKAGED_BINARIES} PROPERTIES GENERATED TRUE)
source_group("Binaries" FILES ${PACKAGED_BINARIES})
list(APPEND RESOURCE_FILES ${PACKAGED_BINARIES})

set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

include_directories(${PROJECT_SOURCE_DIR}/Runtime ${PROJECT_BINARY_DIR}/Runtime)
link_directories(${WEBKIT_LINK_DIRECTORIES})

add_executable(TestRunner WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(TestRunner Runtime)
add_dependencies(TestRunner TestFixtures)

set_target_properties(TestRunner PROPERTIES
    VS_WINRT_COMPONENT TRUE
)
