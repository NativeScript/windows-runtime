set(HEADER_FILES
    pch.h
)

set(SOURCE_FILES
    EventsTests.cpp
    InterfacesTests.cpp
    MarshalingTests.cpp
    NamespacesTests.cpp
    OverloadsTests.cpp
    StructsTests.cpp
    ClassesTests.cpp
    CMakeLists.txt
    DelegatesTests.cpp
    EnumsTests.cpp
)

file(GLOB CONTENT_FILES ${PROJECT_SOURCE_DIR}/cmake/Assets/*)
source_group("Assets" FILES ${CONTENT_FILES})

list(APPEND CONTENT_FILES
    Package.appxmanifest
)
set_property(SOURCE ${CONTENT_FILES} PROPERTY VS_DEPLOYMENT_CONTENT 1)

set(RESOURCE_FILES
    ${CONTENT_FILES}
)

source_group("" FILES ${HEADER_FILES})
source_group("TestCases" FILES ${SOURCE_FILES})

include_directories(
    ${PROJECT_SOURCE_DIR}/Metadata
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/UnitTests.vcxproj.user DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(UnitTests SHARED ${SOURCE_FILES} ${HEADER_FILES} ${RESOURCE_FILES})
target_link_libraries(UnitTests Metadata)
add_dependencies(UnitTests TestFixtures)

set_target_properties(UnitTests PROPERTIES LINK_FLAGS "/LIBPATH:$(VCInstallDir)UnitTest\\lib")
set_target_properties(UnitTests PROPERTIES COMPILE_FLAGS "/I$(VCInstallDir)UnitTest\\include")

set_target_properties(UnitTests PROPERTIES
    VS_WINRT_COMPONENT TRUE
)
