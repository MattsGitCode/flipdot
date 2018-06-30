find_package(PkgConfig REQUIRED)
find_package(LibElf REQUIRED)

configure_file(SimAvr.CMakeLists.in SimAvr/CMakeLists.txt @ONLY)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/SimAvr )
if (result)
    message(FATAL_ERROR "CMake step of SimAvr failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/SimAvr)
if (result)
    message(FATAL_ERROR "Build step of SimAvr failed: ${result}")
endif()

add_library(SimAvr STATIC IMPORTED GLOBAL)

find_library(libsimavr
        NAMES libsimavr.a simavr
        PATHS ${CMAKE_CURRENT_BINARY_DIR}/SimAvr/SimAvr-prefix/lib
        NO_DEFAULT_PATH)

set_target_properties(SimAvr PROPERTIES
        IMPORTED_LOCATION ${libsimavr}
        INTERFACE_LINK_LIBRARIES "${LIBELF_LIBRARIES}"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_BINARY_DIR}/SimAvr/SimAvr-prefix/include")
