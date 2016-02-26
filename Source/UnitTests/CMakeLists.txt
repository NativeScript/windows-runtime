set(HEADER_FILES
    UnitTests-Prefix.h
)

set(METADATA_TESTS
    MetadataTests/ClassesTests.cpp
    MetadataTests/DelegatesTests.cpp
    MetadataTests/EnumsTests.cpp
    MetadataTests/EventsTests.cpp
    MetadataTests/InterfacesTests.cpp
    MetadataTests/MarshalingTests.cpp
    MetadataTests/NamespacesTests.cpp
    MetadataTests/OverloadsTests.cpp
    MetadataTests/StructsTests.cpp
)
set(SOURCE_FILES
    ${METADATA_TESTS}
    Main.cpp
    UnitTests-Prefix.cpp
)

source_group("" FILES ${HEADER_FILES})
source_group("MetadataTests" FILES ${METADATA_TESTS})
source_group("" FILES ${SOURCE_FILES})

list(APPEND ${SOURCE_FILES} UnitTests-Prefix.cpp)

file(GLOB CONTENT_FILES ${PROJECT_SOURCE_DIR}/cmake/Assets/*)
source_group("Assets" FILES ${CONTENT_FILES})

set(RESOURCE_FILES
    ${CONTENT_FILES}
)

set(SHORT_NAME UnitTests)
set(PACKAGE_GUID 4961B5D7-2CCC-4060-879C-695F84B1E354)
set(APPLICATION_ID "vstest.executionengine.universal.App")
configure_file(
    ${PROJECT_SOURCE_DIR}/cmake/Package.appxmanifest.in
    ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest
    @ONLY
)
list(APPEND RESOURCE_FILES ${CMAKE_CURRENT_BINARY_DIR}/Package.appxmanifest)

set_property(SOURCE ${RESOURCE_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

include_directories(
    ${PROJECT_SOURCE_DIR}/Metadata
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# TODO
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/UnitTests.vcxproj.user DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(UnitTests WIN32 ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(UnitTests Metadata)
add_dependencies(UnitTests TestFixtures)

include(PrecompiledHeaders)
SET_PRECOMPILED_HEADER(UnitTests UnitTests-Prefix.h UnitTests-Prefix.cpp)

set_target_properties(UnitTests PROPERTIES LINK_FLAGS "/LIBPATH:$(VCInstallDir)UnitTest/lib/UWP")
set_target_properties(UnitTests PROPERTIES COMPILE_FLAGS "/I$(VCInstallDir)UnitTest/include/UWP")

set_target_properties(UnitTests PROPERTIES
    VS_WINRT_COMPONENT TRUE
)
