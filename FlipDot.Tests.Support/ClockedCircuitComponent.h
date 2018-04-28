#include <utility>

#pragma once


#include <cstddef>
#include <cassert>
#include <vector>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <chrono>

#include "ClockedPinValue.h"

class DiscreteClockedCircuitComponent;
class ClockedPin;


class ClockCount {
private:
    size_t circuitTicks;
    size_t virtualTicks;
    std::chrono::nanoseconds elapsedTime;

    ClockCount(size_t circuitTicks, size_t virtualTicks, std::chrono::nanoseconds elapsedTime);

public:
    static const ClockCount Zero;

    ClockCount(const ClockCount &) = default;

    bool Follows(const ClockCount &previous) const;
    bool operator==(const ClockCount &rhs) const;
    bool operator!=(const ClockCount &rhs) const;
    ClockCount Next(size_t virtualCyclesProgressed, std::chrono::nanoseconds timeProgressed) const;
    const std::chrono::nanoseconds TimeSince(const ClockCount &from) const;
};


class ClockedCircuitComponent {
private:
    std::vector<std::reference_wrapper<ClockedPin>> clockedPins;
protected:
    ClockedCircuitComponent();
    void PushCurrentPinValues();
public:
    virtual void Tick(ClockCount) = 0;

    void RegisterPin(ClockedPin &clockedPin);
    virtual void EnsureTicksMatch(ClockedCircuitComponent &otherComponent) = 0;
};

class PrimaryClockedCircuitComponent : public ClockedCircuitComponent {
protected:
    PrimaryClockedCircuitComponent() = default;
public:
    virtual const ClockCount Tick() = 0;
};


class DiscreteClockedCircuitComponent : public ClockedCircuitComponent {
    friend class ClockedPin;

private:
    ClockCount lastProcessedClock = ClockCount::Zero;
    std::string name;

    virtual void Tick() = 0;

protected:
    explicit DiscreteClockedCircuitComponent(std::string name);

public:
    DiscreteClockedCircuitComponent(DiscreteClockedCircuitComponent &&) = delete;
    DiscreteClockedCircuitComponent(const DiscreteClockedCircuitComponent &) = delete;
    virtual DiscreteClockedCircuitComponent &operator=(const DiscreteClockedCircuitComponent &) = delete;
    virtual DiscreteClockedCircuitComponent &operator=(DiscreteClockedCircuitComponent &&) = delete;

    void Tick(ClockCount clockCount) final;

    void EnsureTicksMatch(ClockedCircuitComponent &otherComponent) final;

    const std::string &Name() const;

    inline const ClockCount CurrentClockCount() const {
        return lastProcessedClock;
    }
};