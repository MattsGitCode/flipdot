find_package(Threads REQUIRED)

configure_file(GoogleTest.CMakeLists.in GoogleTest/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/GoogleTest )

if (result)
    message(FATAL_ERROR "CMake step of GoogleTest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/GoogleTest)
if (result)
    message(FATAL_ERROR "Build step of GoogleTest failed: ${result")
endif()


add_library(GoogleTest STATIC IMPORTED GLOBAL)

find_library(libgtest "gtest" ${CMAKE_CURRENT_BINARY_DIR}/GoogleTest/GoogleTest-prefix/src/GoogleTest-build/googlemock/gtest/)

set_target_properties(GoogleTest PROPERTIES
        IMPORTED_LOCATION ${libgtest}
        INTERFACE_LINK_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/GoogleTest/GoogleTest-prefix/src/GoogleTest/googletest/include")