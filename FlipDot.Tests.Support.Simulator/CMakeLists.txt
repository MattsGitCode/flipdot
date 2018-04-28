project(FlipDot.Tests.Support.Simulator)

add_library(FlipDot.Tests.Support.Simulator STATIC
        AvrSimulator.cpp
        SimulatorPorts.h)

target_link_libraries(FlipDot.Tests.Support.Simulator
        SimAvr)

target_include_directories(FlipDot.Tests.Support.Simulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
