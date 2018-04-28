#pragma once

#include <string>
#include <memory>
#include <vector>

#include "SimulatorPorts.h"

struct avr_t;
struct avr_irq_t;

namespace FlipDotTestSupportSimulator {
    typedef std::function<void(const Port port, const uint32_t value)> PortChangeCallback;
    typedef std::function<void(const Port port, const Pin pin, const bool value)> PinChangeCallback;


    class AvrSimulator final {
    private:
        typedef std::function<void(struct avr_irq_t *irq, const uint32_t value)> IrqCallback;

    private:
        const std::unique_ptr<avr_t> simavr_avr;
        int cpuState;
        std::vector<std::unique_ptr<IrqCallback>> irq_callbacks;

        static void IrqCallbackInvoker(struct avr_irq_t *irq, uint32_t value, void *param);

    public:
        AvrSimulator(std::string microcontroller_name, std::string elf_path, size_t frequencyInHertz);
        ~AvrSimulator();

        void RegisterPortChangeCallback(Port port, PortChangeCallback callback);

        void RegisterPortPinChangeCallback(Port port, Pin pin, PinChangeCallback callback);

        void Run();
        void Step();

        bool IsDone() const;
        bool IsCrashed() const;
        size_t Cycles() const;
        size_t ClockFrequency() const;
    };
}
