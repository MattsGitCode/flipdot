#include "Atmega88PA.h"

#include <sstream>
#include <AvrSimulator.h>
#include <ClockedCircuitException.h>

namespace Sim = FlipDotTestSupportSimulator;

FlipDotTestSupport::Atmega88PA::Atmega88PA(std::string elf_path, size_t frequencyInHertz)
        : avrSimulator("atmega88pa", elf_path, frequencyInHertz),
          PortD(*this, Sim::Port::D) {}

const ClockCount FlipDotTestSupport::Atmega88PA::Tick() {
    PushCurrentPinValues();

    size_t cyclesBefore = avrSimulator.Cycles();
    size_t clockFrequency = avrSimulator.ClockFrequency();
    avrSimulator.Step();

    size_t cyclesProgressed = avrSimulator.Cycles() - cyclesBefore;

    std::chrono::nanoseconds timeProgressed = std::chrono::nanoseconds((cyclesProgressed * std::nano::den) / clockFrequency);

    lastProcessed = lastProcessed.Next(cyclesProgressed, timeProgressed);

    return lastProcessed;
}

void FlipDotTestSupport::Atmega88PA::Tick(ClockCount count) {
    if (lastProcessed != count) {
        throw ClockedCircuitComponentException("Primary component Atmega88PA cannot be clocked by another component");
    }
}

void FlipDotTestSupport::Atmega88PA::EnsureTicksMatch(ClockedCircuitComponent &otherComponent) {
    otherComponent.Tick(lastProcessed);
}

inline std::string PinName(Sim::Port port, Sim::Pin pin) {
    std::stringstream ss;
    ss << "Port" << port << ":" << pin;
    return ss.str();
}

FlipDotTestSupport::Atmega88PA::Port::Port(FlipDotTestSupport::Atmega88PA &atmega88PA,
                                           FlipDotTestSupportSimulator::Port port)
        : Pin0(atmega88PA, PinName(port, Sim::Pin::_0)),
          Pin1(atmega88PA, PinName(port, Sim::Pin::_1)),
          Pin2(atmega88PA, PinName(port, Sim::Pin::_2)),
          Pin3(atmega88PA, PinName(port, Sim::Pin::_3)),
          Pin4(atmega88PA, PinName(port, Sim::Pin::_4)),
          Pin5(atmega88PA, PinName(port, Sim::Pin::_5)),
          Pin6(atmega88PA, PinName(port, Sim::Pin::_6)),
          Pin7(atmega88PA, PinName(port, Sim::Pin::_7)) {
    WireupOutputPin(Pin0, atmega88PA, port, Sim::Pin::_0);
    WireupOutputPin(Pin1, atmega88PA, port, Sim::Pin::_1);
    WireupOutputPin(Pin2, atmega88PA, port, Sim::Pin::_2);
    WireupOutputPin(Pin3, atmega88PA, port, Sim::Pin::_3);
    WireupOutputPin(Pin4, atmega88PA, port, Sim::Pin::_4);
    WireupOutputPin(Pin5, atmega88PA, port, Sim::Pin::_5);
    WireupOutputPin(Pin6, atmega88PA, port, Sim::Pin::_6);
    WireupOutputPin(Pin7, atmega88PA, port, Sim::Pin::_7);
}

void FlipDotTestSupport::Atmega88PA::Port::WireupOutputPin(ClockedPin &outputPin,
                                                           FlipDotTestSupport::Atmega88PA &atmega88PA,
                                                           FlipDotTestSupportSimulator::Port port,
                                                           FlipDotTestSupportSimulator::Pin pin) {


    atmega88PA.avrSimulator.RegisterPortPinChangeCallback(port, pin, [&](auto port, auto pin, auto value) {
        outputPin.SetValue(value ? ClockedPinValue::High() : ClockedPinValue::Low());
    });
}

