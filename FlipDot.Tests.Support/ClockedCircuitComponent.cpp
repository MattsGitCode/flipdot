#include <utility>

#include "ClockedCircuitComponent.h"

#include "ClockedPin.h"
#include "ClockedCircuitException.h"


ClockedCircuitComponent::ClockedCircuitComponent()
        : clockedPins() {}

void ClockedCircuitComponent::RegisterPin(ClockedPin &clockedPin) {
    clockedPins.push_back(clockedPin);
}

void ClockedCircuitComponent::PushCurrentPinValues() {
    for (ClockedPin &clockedPin : clockedPins) {
        clockedPin.PushCurrentValue();
    }
}

DiscreteClockedCircuitComponent::DiscreteClockedCircuitComponent(std::string name)
        : name(std::move(name)) {}

void DiscreteClockedCircuitComponent::Tick(ClockCount clock) {
    if (lastProcessedClock == clock) {
        return;
    }

    if (!clock.Follows(lastProcessedClock)) {
        throw ClockedCircuitComponentException(
                "Component ticks are out of sync, make sure all components are added to the ClockedCircuit");
    }

    lastProcessedClock = clock;

    PushCurrentPinValues();

    Tick();
}

void DiscreteClockedCircuitComponent::EnsureTicksMatch(ClockedCircuitComponent &otherComponent) {
    otherComponent.Tick(lastProcessedClock);
}

const std::string &DiscreteClockedCircuitComponent::Name() const {
    return name;
}

ClockCount::ClockCount(size_t circuitTicks, size_t virtualTicks, std::chrono::nanoseconds elapsedTime)
        : circuitTicks(circuitTicks),
          virtualTicks(virtualTicks),
          elapsedTime(elapsedTime) {}

const ClockCount ClockCount::Zero = ClockCount(0, 0, std::chrono::nanoseconds::zero());

bool ClockCount::operator==(const ClockCount &rhs) const {
    return circuitTicks == rhs.circuitTicks &&
           virtualTicks == rhs.virtualTicks;
}

bool ClockCount::operator!=(const ClockCount &rhs) const {
    return !this->operator==(rhs);
}

bool ClockCount::Follows(const ClockCount &previous) const {
    return circuitTicks == (previous.circuitTicks + 1);
}

ClockCount ClockCount::Next(size_t virtualCyclesProgressed, std::chrono::nanoseconds timeProgressed) const {
    return ClockCount(circuitTicks + 1, virtualTicks + virtualCyclesProgressed, elapsedTime + timeProgressed);
}

const std::chrono::nanoseconds ClockCount::TimeSince(const ClockCount &from) const {
    return elapsedTime - from.elapsedTime;
}
