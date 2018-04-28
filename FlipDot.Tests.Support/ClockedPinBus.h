#pragma once


#include <cstddef>
#include <type_traits>
#include <string>
#include <tuple>
#include <cassert>
#include <iostream>
#include <tuple>

#include "ClockedPin.h"
#include "ClockedPinBusBase.h"



template<size_t busSize>
class ClockedPinBus : private ClockedPinBus<busSize - 1>,
                      public ClockedPinBusBase<ClockedPinBus<busSize>, busSize> {
public:
    using ClockedPinBusBase<ClockedPinBus, busSize>::WithReversedOrder;
    using ClockedPinBusBase<ClockedPinBus, busSize>::WithPartial;
    using ClockedPinBusBase<ClockedPinBus, busSize>::OutputTo;
    using ClockedPinBusBase<ClockedPinBus, busSize>::ToBitset;
    using ClockedPinBusBase<ClockedPinBus, busSize>::SetValues;

private:
    ClockedPin pin;

public:
    ClockedPinBus(DiscreteClockedCircuitComponent &component, std::string name)
            : ClockedPinBus<busSize - 1>(component, name),
              pin(component, name + std::to_string(busSize - 1)) {}


    template<size_t pinIndex, typename std::enable_if<pinIndex == (busSize - 1)>::type * = nullptr>
    constexpr inline ClockedPin &PinAt() {
        return pin;
    }

    template<size_t pinIndex, typename std::enable_if<pinIndex < (busSize - 1)>::type * = nullptr>
    constexpr inline ClockedPin &PinAt() {
        return ClockedPinBus<busSize - 1>::template PinAt<pinIndex>();
    }

    template<size_t pinIndex, typename std::enable_if<(pinIndex >= busSize)>::type * = nullptr>
    ClockedPin &PinAt() const {
        static_assert(pinIndex < busSize, "pinIndex must be less than busSize");
    }
};

template<>
class ClockedPinBus<0> {
public:
    ClockedPinBus(DiscreteClockedCircuitComponent &component, std::string name) {}
};







template<class ...BusTypes>
struct SumBusSizes;

template<>
struct SumBusSizes<> {
    constexpr static size_t BusSize = 0;
};

template<template<size_t busSize> class BusType, size_t busSize, class ...BusTypes>
struct SumBusSizes<BusType<busSize>, BusTypes...> {
    constexpr static size_t BusSize = busSize + SumBusSizes<BusTypes...>::BusSize;
};

template<template<size_t busSize> class BusType, size_t busSize, class ...BusTypes>
struct SumBusSizes<BusType<busSize> &, BusTypes...> {
    constexpr static size_t BusSize = busSize + SumBusSizes<BusTypes...>::BusSize;
};


template<class ...BusType>
class CompositePinBus;


template<>
class CompositePinBus<> {
};


template<template<size_t busSize> class BusType, size_t busSize, class ...BusTypes>
class CompositePinBus<BusType<busSize>, BusTypes...>
        : private CompositePinBus<BusTypes...>,
          public ClockedPinBusBase<CompositePinBus<BusType<busSize>, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize> {

public:
    using ClockedPinBusBase<CompositePinBus<BusType<busSize>, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize>::WithReversedOrder;
    using ClockedPinBusBase<CompositePinBus<BusType<busSize>, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize>::WithPartial;
    using ClockedPinBusBase<CompositePinBus<BusType<busSize>, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize>::OutputTo;

private:
    BusType<busSize> pinBus;
public:
    template<class BusType2, class ...BusTypes2>
    explicit CompositePinBus(BusType2 &&bus, BusTypes2 &&...buses)
            : CompositePinBus<BusTypes2...>(std::forward<BusTypes2>(buses)...),
              pinBus(std::forward<BusType2>(bus)) {}


    template<size_t pinIndex, typename std::enable_if<pinIndex < busSize>::type * = nullptr>
    constexpr inline ClockedPin &PinAt() const {
        return pinBus.PinAt<pinIndex>();
    }

    template<size_t pinIndex, typename std::enable_if<pinIndex >= busSize>::type * = nullptr>
    constexpr inline ClockedPin &PinAt() const {
        return CompositePinBus<BusTypes...>::template PinAt<pinIndex - busSize>();
    }
};

template<template<size_t busSize> class BusType, size_t busSize, class ...BusTypes>
class CompositePinBus<BusType<busSize> &, BusTypes...>
        : private CompositePinBus<BusTypes...>,
          public ClockedPinBusBase<CompositePinBus<BusType<busSize> &, BusTypes...>, SumBusSizes<BusType<busSize> &, BusTypes...>::BusSize> {
public:
    using ClockedPinBusBase<CompositePinBus<BusType<busSize> &, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize>::WithReversedOrder;
    using ClockedPinBusBase<CompositePinBus<BusType<busSize> &, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize>::WithPartial;
    using ClockedPinBusBase<CompositePinBus<BusType<busSize> &, BusTypes...>, SumBusSizes<BusType<busSize>, BusTypes...>::BusSize>::OutputTo;

private:
    BusType<busSize> &pinBus;
public:
    template<class BusType2, class ...BusTypes2>
    explicit CompositePinBus(BusType2 &&bus, BusTypes2 &&...buses)
            : CompositePinBus<BusTypes2...>(std::forward<BusTypes2>(buses)...),
              pinBus(std::forward<BusType2>(bus)) {}


    template<size_t pinIndex, typename std::enable_if<pinIndex < busSize>::type * = nullptr>
    constexpr inline ClockedPin &PinAt() const {
        return pinBus.PinAt<pinIndex>();
    }

    template<size_t pinIndex, typename std::enable_if<pinIndex >= busSize>::type * = nullptr>
    constexpr inline ClockedPin &PinAt() const {
        return CompositePinBus<BusTypes...>::template PinAt<pinIndex - busSize>();
    }
};


template<class ...BusType>
auto CreateCompositeBus(BusType &&...buses) {
    return CompositePinBus<BusType...>(std::forward<BusType>(buses)...);
}
