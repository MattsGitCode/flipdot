#pragma once

#include <string>

#include "ClockedPinValue.h"

class ClockedCircuitComponent;

class ClockedPin final {
    friend class ClockedCircuitComponent;

private:
    std::string name;
    ClockedCircuitComponent &component;

    ClockedPinValue previousValue;
    ClockedPinValue value;
    const ClockedPin *inputFrom = nullptr;

    const ClockedPin &InputFrom() const;

    void PushCurrentValue();

public:
    explicit ClockedPin(ClockedCircuitComponent &component, std::string name);

    inline std::string Name() const {
        return name;
    }

    ClockedPin(const ClockedPin &) = delete;
    ClockedPin(ClockedPin &&) = delete;
    ClockedPin operator=(const ClockedPin &) = delete;
    ClockedPin operator=(ClockedPin &&) = delete;


    void SetValue(const ClockedPinValue &pinValue);

    const ClockedPinValue Value() const;

    void InputFrom(const ClockedPin *outputPin);

    inline void SameInputAs(const ClockedPin &other) {
        InputFrom(other.inputFrom);
    }

    void OutputTo(ClockedPin &inputPin) const;
};


