cmake_minimum_required(VERSION 3.0.0)
project(NativeScript)

add_definitions(
    # Windows 8.1
    /D_WIN32_WINNT=0x0603
    /DWINVER=0x0603
    /DNOMINMAX
    /wd4018 /wd4068 /wd4099 /wd4100 /wd4127 /wd4138 /wd4146 /wd4180 /wd4189 /wd4201 /wd4244 /wd4251 /wd4267 /wd4275 /wd4288
    /wd4291 /wd4305 /wd4309 /wd4344 /wd4355 /wd4389 /wd4396 /wd4481 /wd4503 /wd4505 /wd4510 /wd4512 /wd4530 /wd4610 /wd4702
    /wd4706 /wd4800 /wd4819 /wd4951 /wd4952 /wd4996 /wd6011 /wd6031 /wd6211 /wd6246 /wd6255 /wd6387
)

include(WebKit.cmake)

add_subdirectory(Metadata)
add_subdirectory(Runtime)

include_external_msproject(TestFixtures ${CMAKE_CURRENT_SOURCE_DIR}/TestFixtures/TestFixtures.csproj)
add_subdirectory(TestRunner)
add_subdirectory(UnitTests)
