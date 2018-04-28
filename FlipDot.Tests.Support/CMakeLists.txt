project(FlipDot.Tests.Support)

set(SOURCES
        ClockedCircuitException.h
        ClockedCircuit.cpp
        ClockedCircuitComponent.cpp
        ClockedPinValue.h
        ClockedPin.cpp
        ClockedPinBusBase.h
        ClockedPinBus.h
        DerivedClockedPinBus.h
        Components/SerialToParallel_74HC595.cpp
        Components/FlipDotDisplay.h
        Components/Atmega88PA.cpp)

add_library(FlipDot.Tests.Support ${SOURCES})

target_link_libraries(FlipDot.Tests.Support
        FlipDot.Tests.Support.Simulator)

target_include_directories(FlipDot.Tests.Support.Simulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
