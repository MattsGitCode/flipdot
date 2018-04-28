#include "SerialToParallel_74HC595.h"

#include <functional>

ClockedPinValue PinValueOf(const bool value) {
    return value ? ClockedPinValue::High() : ClockedPinValue::Low();
}

void FlipDotTestSupport::SerialToParallel_74HC595::Tick() {
    if (ClockPin.Value().IsRisingEdge()) {
        SerialOutPin.SetValue(PinValueOf(unlatchedValue.test(7)));

        unlatchedValue <<= 1;
        unlatchedValue.set(0, DataPin.Value().IsHigh());

        if (LatchPin.Value().IsHigh()) {
            latchedValue = unlatchedValue;

            if (OutputEnablePin.Value().IsLow()) {
                OutputPinBus.SetValues(latchedValue);
            }
        }
    } else  if (LatchPin.Value().IsRisingEdge()) {
        latchedValue = unlatchedValue;

        if (OutputEnablePin.Value().IsLow()) {
            OutputPinBus.SetValues(latchedValue);
        }
    } else if (OutputEnablePin.Value().IsFallingEdge()) {
        OutputPinBus.SetValues(latchedValue);
    } else if (OutputEnablePin.Value().IsRisingEdge()) {
        OutputPinBus.SetValues(std::bitset<8>());
    }
}

FlipDotTestSupport::SerialToParallel_74HC595::CascadeBuilder
FlipDotTestSupport::SerialToParallel_74HC595::CascadeTo(FlipDotTestSupport::SerialToParallel_74HC595 &next) {
    CascadeBuilder cascadeBuilder(*this);
    cascadeBuilder.AndThenTo(next);

    return cascadeBuilder;


}

FlipDotTestSupport::SerialToParallel_74HC595::CascadeBuilder::CascadeBuilder(SerialToParallel_74HC595 &current)
        : cascaded({current}) {

}

FlipDotTestSupport::SerialToParallel_74HC595::CascadeBuilder
FlipDotTestSupport::SerialToParallel_74HC595::CascadeBuilder::AndThenTo(
        FlipDotTestSupport::SerialToParallel_74HC595 &next) {

    SerialToParallel_74HC595 &previous = cascaded.back();

    previous.SerialOutPin.OutputTo(next.DataPin);
    next.ClockPin.SameInputAs(previous.ClockPin);
    next.LatchPin.SameInputAs(previous.LatchPin);
    next.OutputEnablePin.SameInputAs(previous.OutputEnablePin);

    cascaded.push_back(next);

    return *this;
}
