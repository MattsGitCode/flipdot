#pragma once

#include <Components/FlipDotDisplay.h>
#include <Components/SerialToParallel_74HC595.h>
#include <Components/Atmega88PA.h>

class FlipDotCircuit {
private:
    FlipDotTestSupport::Atmega88PA atmega88PA;
    FlipDotTestSupport::Peripherals::FlipDotDisplay<30, 8> display;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnNegative30_25;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnPositive30_25;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnNegative24_17;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnPositive24_17;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnNegative16_09;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnPositive16_09;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnNegative08_01;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterColumnPositive08_01;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterRowNegative01_08;
    FlipDotTestSupport::SerialToParallel_74HC595 serialShifterRowPositive01_08;

public:
    explicit FlipDotCircuit(std::string elfPath);

    FlipDotTestSupport::Peripherals::FlipDotDisplay<30, 8> &FlipDotDisplay();

    void Run();
};


