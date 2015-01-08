set(SOURCE_FILES main.cpp)

include_directories(${WEBKIT_INCLUDE_DIRECTORIES})
link_directories(${WEBKIT_LINK_DIRECTORIES})

add_library(Runtime SHARED ${SOURCE_FILES})
add_dependencies(Runtime WebKit)
target_link_libraries(Runtime JavaScriptCore)