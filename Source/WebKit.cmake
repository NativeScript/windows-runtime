set(WEBKIT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/WebKit")
set(WTF_SOURCE_DIR "${WEBKIT_SOURCE_DIR}/Source/WTF")
set(JavaScriptCore_SOURCE_DIR "${WEBKIT_SOURCE_DIR}/Source/JavaScriptCore")
set(JavaScriptCore_INCLUDE_DIRECTORIES
    "${JavaScriptCore_SOURCE_DIR}"
    "${JavaScriptCore_SOURCE_DIR}/ForwardingHeaders"
    "${JavaScriptCore_SOURCE_DIR}/API"
    "${JavaScriptCore_SOURCE_DIR}/assembler"
    "${JavaScriptCore_SOURCE_DIR}/bytecode"
    "${JavaScriptCore_SOURCE_DIR}/bytecompiler"
    "${JavaScriptCore_SOURCE_DIR}/dfg"
    "${JavaScriptCore_SOURCE_DIR}/disassembler"
    "${JavaScriptCore_SOURCE_DIR}/heap"
    "${JavaScriptCore_SOURCE_DIR}/debugger"
    "${JavaScriptCore_SOURCE_DIR}/interpreter"
    "${JavaScriptCore_SOURCE_DIR}/jit"
    "${JavaScriptCore_SOURCE_DIR}/llint"
    "${JavaScriptCore_SOURCE_DIR}/parser"
    "${JavaScriptCore_SOURCE_DIR}/profiler"
    "${JavaScriptCore_SOURCE_DIR}/runtime"
    "${JavaScriptCore_SOURCE_DIR}/yarr"
)

set(WEBKIT_CMAKE_ARGS
    -DPORT:STRING=WinRT
    -DENABLE_WEBCORE:BOOL=OFF
    -DENABLE_WEBKIT:BOOL=OFF
    -DENABLE_WEBKIT2:BOOL=OFF
    -DENABLE_INSPECTOR:BOOL=ON
    -DUSE_SYSTEM_MALLOC:BOOL=ON
    -DWTF_USE_ICU_UNICODE:BOOL=OFF
    -DWTF_USE_NLS_UNICODE:BOOL=ON
    -DWTF_USE_CF:BOOL=OFF
    -DSHARED_CORE:BOOL=ON
    -DSHOULD_INSTALL_JS_SHELL:BOOL=OFF
    -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
    -DCMAKE_SYSTEM_NAME:STRING=${CMAKE_SYSTEM_NAME}
    -DCMAKE_SYSTEM_VERSION:STRING=${CMAKE_SYSTEM_VERSION}
    -Wno-dev
)

include(ExternalProject)
ExternalProject_Add(
    WebKit
    SOURCE_DIR ${WEBKIT_SOURCE_DIR}
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    CMAKE_ARGS ${WEBKIT_CMAKE_ARGS}
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --config $<CONFIG>
)

get_property(WEBKIT_BINARY_DIR TARGET WebKit PROPERTY _EP_BINARY_DIR)
get_property(WEBKIT_INSTALL_DIR TARGET WebKit PROPERTY _EP_INSTALL_DIR)

set(WEBKIT_INCLUDE_DIRECTORIES
    "${WEBKIT_SOURCE_DIR}/Source"
    "${WTF_SOURCE_DIR}"
    ${JavaScriptCore_INCLUDE_DIRECTORIES}
    "${WEBKIT_BINARY_DIR}"
    "${WEBKIT_BINARY_DIR}/DerivedSources"
    "${WEBKIT_BINARY_DIR}/DerivedSources/ForwardingHeaders"
    "${WEBKIT_BINARY_DIR}/DerivedSources/JavaScriptCore"
)

set(WEBKIT_LINK_DIRECTORIES ${WEBKIT_INSTALL_DIR}/lib)
set(WEBKIT_LIBRARIES WTF JavaScriptCore)

set(WEBKIT_BINARIES
    ${WEBKIT_INSTALL_DIR}/lib/WTF.dll
    ${WEBKIT_INSTALL_DIR}/lib/JavaScriptCore.dll
)

add_definitions(-DBUILDING_WITH_CMAKE=1)
add_definitions(-DHAVE_CONFIG_H=1)
