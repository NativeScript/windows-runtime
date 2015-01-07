cmake_minimum_required(VERSION 3.0.0)
project(NativeScript)

include_external_msproject(TestFixtures ${CMAKE_CURRENT_SOURCE_DIR}/TestFixtures/TestFixtures.csproj)
add_subdirectory(Metadata)
add_subdirectory(TestRunner)
add_subdirectory(UnitTests)
