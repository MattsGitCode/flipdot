cmake_minimum_required(VERSION 3.5)
project(FlipDot.Core)

set(CMAKE_CXX_STANDARD 14)

set(source library.cpp)

add_library(FlipDot.Core ${source})

target_include_directories(FlipDot.Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})