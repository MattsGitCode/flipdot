#include "ClockedCircuit.h"

#include <iostream>
#include "ClockedCircuitComponent.h"

ClockedCircuit::ClockedCircuit(PrimaryClockedCircuitComponent &primaryComponent,
                               std::initializer_list<std::reference_wrapper<ClockedCircuitComponent>> components)
        : primaryComponent(primaryComponent),
          components(components) {}

void ClockedCircuit::RunWhile(std::function<bool()> runWhileTrue) {
    ClockCount clockCount = ClockCount::Zero;

    while (runWhileTrue()) {

        clockCount = primaryComponent.Tick();

        for (ClockedCircuitComponent &component : components) {
            component.Tick(clockCount);
        }
    }

    std::cout << "Total Elapsed Time: " << std::chrono::duration_cast<std::chrono::seconds>(clockCount.TimeSince(ClockCount::Zero)).count() << " seconds" << std::endl;
}
