#include "FlipDotCircuit.h"


#include <ClockedCircuit.h>

#define nameof(x) #x

FlipDotCircuit::FlipDotCircuit(std::string elfPath)
        : atmega88PA(elfPath, 8000000),
          serialShifterColumnNegative30_25(nameof(serialShifterColumnNegative30_25)),
          serialShifterColumnPositive30_25(nameof(serialShifterColumnPositive30_25)),
          serialShifterColumnNegative24_17(nameof(serialShifterColumnNegative24_17)),
          serialShifterColumnPositive24_17(nameof(serialShifterColumnPositive24_17)),
          serialShifterColumnNegative16_09(nameof(serialShifterColumnNegative16_09)),
          serialShifterColumnPositive16_09(nameof(serialShifterColumnPositive16_09)),
          serialShifterColumnNegative08_01(nameof(serialShifterColumnNegative08_01)),
          serialShifterColumnPositive08_01(nameof(serialShifterColumnPositive08_01)),
          serialShifterRowNegative01_08(nameof(serialShifterRowNegative01_08)),
          serialShifterRowPositive01_08(nameof(serialShifterRowPositive01_08)) {


    atmega88PA.PortD.Pin2.OutputTo(serialShifterColumnNegative30_25.DataPin);
    atmega88PA.PortD.Pin0.OutputTo(serialShifterColumnNegative30_25.ClockPin);
    atmega88PA.PortD.Pin1.OutputTo(serialShifterColumnNegative30_25.LatchPin);
    atmega88PA.PortD.Pin3.OutputTo(serialShifterColumnNegative30_25.OutputEnablePin);

    serialShifterColumnNegative30_25
            .CascadeTo(serialShifterColumnPositive30_25)
            .AndThenTo(serialShifterColumnNegative24_17)
            .AndThenTo(serialShifterColumnPositive24_17)
            .AndThenTo(serialShifterColumnNegative16_09)
            .AndThenTo(serialShifterColumnPositive16_09)
            .AndThenTo(serialShifterColumnNegative08_01)
            .AndThenTo(serialShifterColumnPositive08_01)
            .AndThenTo(serialShifterRowNegative01_08)
            .AndThenTo(serialShifterRowPositive01_08);

    auto columnPositiveBus = CreateCompositeBus(
            serialShifterColumnPositive30_25.OutputPinBus,
            serialShifterColumnPositive24_17.OutputPinBus,
            serialShifterColumnPositive16_09.OutputPinBus,
            serialShifterColumnPositive08_01.OutputPinBus)
            .WithReversedOrder()
            .WithPartial<0, 29>();

    auto columnNegativeBus = CreateCompositeBus(
            serialShifterColumnNegative30_25.OutputPinBus,
            serialShifterColumnNegative24_17.OutputPinBus,
            serialShifterColumnNegative16_09.OutputPinBus,
            serialShifterColumnNegative08_01.OutputPinBus)
            .WithReversedOrder()
            .WithPartial<0, 29>();

    columnPositiveBus.OutputTo(display.columnPositiveBus);
    columnNegativeBus.OutputTo(display.columnNegativeBus);
    serialShifterRowPositive01_08.OutputPinBus.OutputTo(display.rowPositiveBus);
    serialShifterRowNegative01_08.OutputPinBus.OutputTo(display.rowNegativeBus);
}

FlipDotTestSupport::Peripherals::FlipDotDisplay<30, 8> &FlipDotCircuit::FlipDotDisplay() {
    return display;
}

void FlipDotCircuit::Run() {
    ClockedCircuit(atmega88PA,
                   {serialShifterColumnPositive30_25,
                    serialShifterColumnNegative24_17,
                    serialShifterColumnPositive24_17,
                    serialShifterColumnNegative16_09,
                    serialShifterColumnPositive16_09,
                    serialShifterColumnNegative08_01,
                    serialShifterColumnPositive08_01,
                    serialShifterRowNegative01_08,
                    serialShifterRowPositive01_08,
                    display})
            .RunWhile([&]() { return !atmega88PA.IsDone(); });
}
