cmake_minimum_required(VERSION 3.5)
project(FlipDot)

enable_testing()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(ThirdParty)

add_subdirectory(FlipDot.Tests.Support.Simulator)
add_subdirectory(FlipDot.Tests.Support)

add_subdirectory(FlipDot.Core)

add_subdirectory(FlipDot.Core.Tests)
