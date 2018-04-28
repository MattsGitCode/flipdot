#include <sstream>
#include <iostream>

#include "ClockedPin.h"
#include "ClockedCircuitException.h"
#include "ClockedCircuitComponent.h"

ClockedPin::ClockedPin(ClockedCircuitComponent &component, std::string name)
        : component(component),
          name(name),
          previousValue(ClockedPinValue::Low()),
          value(ClockedPinValue::Low()) {
    component.RegisterPin(*this);
}

void ClockedPin::PushCurrentValue() {
    previousValue = value;
    value = value.IsHigh() ? ClockedPinValue::High() : ClockedPinValue::Low();
}

void ClockedPin::SetValue(const ClockedPinValue &pinValue) {
    if (inputFrom != nullptr) {
        throw ClockedCircuitComponentException("Pin has an input connection and cannot be set directly");
    }

    value = ClockedPinValue(previousValue, pinValue);
}

const ClockedPinValue ClockedPin::Value() const {
    component.EnsureTicksMatch(InputFrom().component);

    return InputFrom().value;
}

void ClockedPin::InputFrom(const ClockedPin *outputPin) {
    if (inputFrom != nullptr) {
        std::stringstream ss;
        ss << "Pin '" << Name() << "' already has a connection to pin '" << inputFrom->Name() << "'";
        throw ClockedCircuitComponentException(ss.str());
    }
    inputFrom = outputPin;
}

const ClockedPin &ClockedPin::InputFrom() const {
    if (inputFrom == nullptr) {
        std::stringstream ss;
        ss << "Component tried to read from pin '" << Name() << "' with no connection";
        throw ClockedCircuitComponentException(ss.str());
    }

    return *inputFrom;
}

void ClockedPin::OutputTo(ClockedPin &inputPin) const {
    inputPin.InputFrom(this);
}
