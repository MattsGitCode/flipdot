#pragma once

#include <cstddef>
#include <bitset>

#include "ClockedPin.h"
#include "DerivedClockedPinBus.h"


template<size_t busSize>
struct ClockedPinBusBitSetBuilder {
    std::bitset<busSize> &bitset;

    template<size_t pinIndex>
    inline void Do(ClockedPin &pin) {
        bitset.set(pinIndex, pin.Value().IsHigh());
    }
};

template<size_t busSize>
struct ClockedPinBusSetFromBitset {
    std::bitset<busSize> &bitset;

    template<size_t pinIndex>
    inline void Do(ClockedPin &pin) {
        pin.SetValue(bitset.test(pinIndex) ? ClockedPinValue::High() : ClockedPinValue::Low());
    }
};

template<class TargetBusType, size_t busSize>
struct ClockedPinBusOutputOperation {
    TargetBusType &targetBus;

    template<size_t pinIndex>
    inline void Do(ClockedPin &pin) {
        pin.OutputTo(targetBus.template PinAt<pinIndex>());
    }
};


template<class BusType, size_t busSize>
class ClockedPinBusBase {
public:
    template<unsigned long firstPin, unsigned long lastPin, typename PinIndices = std::make_index_sequence<
            lastPin - firstPin + 1>>
    DerivedClockedPinBus<lastPin - firstPin + 1> WithPartial() {
        static_assert(firstPin < busSize, "firstPin must be less than busSize");
        static_assert(lastPin < busSize, "lastPin must be less than busSize");
        static_assert(firstPin <= lastPin, "firstPin must be less than or equal to lastPin");

        return WithPartialImpl<firstPin, lastPin>(PinIndices());
    }
private:
    template<size_t firstPin, size_t lastPin, size_t ...pinIndex>
    DerivedClockedPinBus<lastPin - firstPin + 1> WithPartialImpl(std::index_sequence<pinIndex...>) {
        return {((BusType *) this)->template PinAt<firstPin + pinIndex>()...};
    }


public:
    template<typename PinIndices = std::make_index_sequence<busSize>>
    DerivedClockedPinBus<busSize> WithReversedOrder() {
        return WithReversedOrderImpl(PinIndices());
    }
private:
    template<size_t ...pinIndex>
    DerivedClockedPinBus<busSize> WithReversedOrderImpl(std::index_sequence<pinIndex...>) {
        return {((BusType *) this)->template PinAt<busSize - 1 - pinIndex>()...};
    }


public:
    template<template<unsigned long destinationBusSize> class DestinationBusType, unsigned long destinationBusSize>
    void OutputTo(DestinationBusType<destinationBusSize> &destinationBus) {
        static_assert(destinationBusSize == busSize, "Destination bus must be of the same size");

        ClockedPinBusOutputOperation<DestinationBusType<destinationBusSize>, busSize> operation{destinationBus};
        ForEachPin(operation);
    }

    std::bitset<busSize> ToBitset() const {
        std::bitset<busSize> bitset;
        bitset.reset();
        ClockedPinBusBitSetBuilder<busSize> bitsetBuilder{bitset};
        ForEachPin(bitsetBuilder);

        return bitset;
    }

    void SetValues(std::bitset<busSize> values) {
        ClockedPinBusSetFromBitset<busSize> operation{values};
        ForEachPin(operation);
    }

private:
    template<class Delegate>
    void ForEachPin(Delegate &delegate) const {
        ForEachPinIndex<Delegate, busSize - 1>(delegate);
    }

    template<class Delegate, size_t pinIndex, typename std::enable_if<(pinIndex > 0)>::type * = nullptr>
    void ForEachPinIndex(Delegate &delegate) const {
        delegate.template Do<pinIndex>(((BusType *) this)->template PinAt<pinIndex>());
        ForEachPinIndex<Delegate, pinIndex - 1>(delegate);
    }

    template<class Delegate, size_t pinIndex, typename std::enable_if<(pinIndex == 0)>::type * = nullptr>
    void ForEachPinIndex(Delegate &delegate) const {
        delegate.template Do<pinIndex>(((BusType *) this)->template PinAt<pinIndex>());
    }
};
