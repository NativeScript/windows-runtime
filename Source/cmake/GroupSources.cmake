macro(GroupSources _currentDir)
    file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/${_currentDir} ${CMAKE_CURRENT_SOURCE_DIR}/${_currentDir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${_currentDir}/${child})
            GroupSources(${_currentDir}/${child})
        else()
            string(REPLACE "/" "\\" groupName ${_currentDir})
            source_group(${groupName} FILES ${CMAKE_CURRENT_SOURCE_DIR}/${_currentDir}/${child})
        endif()
    endforeach()
endmacro()