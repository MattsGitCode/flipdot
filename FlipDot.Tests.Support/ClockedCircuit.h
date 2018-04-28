#pragma once

#include <functional>
#include <vector>

class ClockedCircuitComponent;
class PrimaryClockedCircuitComponent;

class ClockedCircuit final {
private:
    PrimaryClockedCircuitComponent &primaryComponent;
    std::vector<std::reference_wrapper<ClockedCircuitComponent>> components;

public:
    ClockedCircuit(PrimaryClockedCircuitComponent &primaryComponent, std::initializer_list<std::reference_wrapper<ClockedCircuitComponent>> components);

    ClockedCircuit(const ClockedCircuit &) = delete;

    void RunWhile(std::function<bool()> runWhileTrue);
};


