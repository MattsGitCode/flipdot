#pragma once

#include <cstddef>
#include <array>

template<class BusType, size_t busSize>
class ClockedPinBusBase;

class ClockedPin;

template<size_t busSize>
class DerivedClockedPinBus
        : public ClockedPinBusBase<DerivedClockedPinBus<busSize>, busSize> {

    template<class BusType, size_t busSize2> friend
    class ClockedPinBusBase;

private:
    std::array<std::reference_wrapper<ClockedPin>, busSize> pins;

    template<typename ...ClockedPin>
    DerivedClockedPinBus(ClockedPin &...clockedPin)
            : pins({clockedPin...}) {}

public:

    template<size_t pinIndex>
    constexpr inline ClockedPin &PinAt() const {
        return std::get<pinIndex>(pins);
    }
};
