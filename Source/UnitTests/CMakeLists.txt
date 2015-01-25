set(HEADER_FILES
    UnitTests-Prefix.h
)

set(METADATA_TESTS
    ClassesTests.cpp
    DelegatesTests.cpp
    EnumsTests.cpp
    EventsTests.cpp
    InterfacesTests.cpp
    MarshalingTests.cpp
    NamespacesTests.cpp
    OverloadsTests.cpp
    StructsTests.cpp
)
set(SOURCE_FILES
    ${METADATA_TESTS}
    UnitTests-Prefix.cpp
)

source_group("" FILES ${HEADER_FILES})
source_group("MetadataTests" FILES ${METADATA_TESTS})
source_group("" FILES ${SOURCE_FILES})

list(APPEND ${SOURCE_FILES} UnitTests-Prefix.cpp)

file(GLOB CONTENT_FILES ${PROJECT_SOURCE_DIR}/cmake/Assets_${CMAKE_SYSTEM_NAME}/*)
source_group("Assets" FILES ${CONTENT_FILES})

list(APPEND CONTENT_FILES
    cmake/Package_${CMAKE_SYSTEM_NAME}.appxmanifest
)
set_property(SOURCE ${CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

set(RESOURCE_FILES
    ${CONTENT_FILES}
)

include_directories(
    ${PROJECT_SOURCE_DIR}/Metadata
)

# TODO
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/cmake/UnitTests_${CMAKE_SYSTEM_NAME}.vcxproj.user DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(RENAME ${CMAKE_CURRENT_BINARY_DIR}/UnitTests_${CMAKE_SYSTEM_NAME}.vcxproj.user ${CMAKE_CURRENT_BINARY_DIR}/UnitTests.vcxproj.user)

add_library(UnitTests SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(UnitTests Metadata)
add_dependencies(UnitTests TestFixtures)

include(PrecompiledHeaders)
SET_PRECOMPILED_HEADER(UnitTests UnitTests-Prefix.h UnitTests-Prefix.cpp)

set_target_properties(UnitTests PROPERTIES LINK_FLAGS "/LIBPATH:$(VCInstallDir)UnitTest/lib")
set_target_properties(UnitTests PROPERTIES COMPILE_FLAGS "/I$(VCInstallDir)UnitTest/include")

set_target_properties(UnitTests PROPERTIES
    VS_WINRT_COMPONENT TRUE
)
