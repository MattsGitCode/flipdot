#include "AvrSimulator.h"

#include <simavr/sim_avr.h>
#include <simavr/sim_elf.h>
#include <simavr/sim_irq.h>
#include <simavr/sim_io.h>
#include <simavr/avr_ioport.h>
#include <iostream>
#include <vector>
#include <functional>
#include <memory>

FlipDotTestSupportSimulator::AvrSimulator::AvrSimulator(const std::string microcontroller_name,
                                                        const std::string elf_path,
                                                        const size_t frequencyInHertz)
        : simavr_avr(avr_make_mcu_by_name(microcontroller_name.c_str())),
          cpuState(cpu_Running),
          irq_callbacks(std::vector<std::unique_ptr<IrqCallback>>()) {

    if (simavr_avr == nullptr) {
        throw "Could not make AVR Simulator for requested microcontroller";
    }

    avr_init(simavr_avr.get());
    simavr_avr->frequency = frequencyInHertz;

    elf_firmware_t simavr_firmware;
    if (elf_read_firmware(elf_path.c_str(), &simavr_firmware) != 0) {
        throw "Could not read firmware file as supplied path";
    }

    avr_load_firmware(simavr_avr.get(), &simavr_firmware);
}

FlipDotTestSupportSimulator::AvrSimulator::~AvrSimulator() {
    avr_terminate(simavr_avr.get());
}


void FlipDotTestSupportSimulator::AvrSimulator::Run() {
    while (!(IsDone() || IsCrashed())) {
        Step();
    }
}

void FlipDotTestSupportSimulator::AvrSimulator::Step() {
    if (IsDone() || IsCrashed()) {
        throw "CPU has finished";
    }

    cpuState = avr_run(simavr_avr.get());
}

inline bool FlipDotTestSupportSimulator::AvrSimulator::IsDone() const {
    return cpuState == cpu_Done;
}

inline bool FlipDotTestSupportSimulator::AvrSimulator::IsCrashed() const {
    return cpuState == cpu_Crashed;
}

size_t FlipDotTestSupportSimulator::AvrSimulator::Cycles() const {
    return simavr_avr->cycle;
}

size_t FlipDotTestSupportSimulator::AvrSimulator::ClockFrequency() const {
    return simavr_avr->frequency;
}


void
FlipDotTestSupportSimulator::AvrSimulator::IrqCallbackInvoker(struct avr_irq_t *irq, uint32_t value, void *param) {
    auto callback = (IrqCallback *) param;
    (*callback)(irq, value);
}

void FlipDotTestSupportSimulator::AvrSimulator::RegisterPortChangeCallback(
        const FlipDotTestSupportSimulator::Port port,
        const PortChangeCallback callback) {

    const auto irq_port = AVR_IOCTL_IOPORT_GETIRQ((char) port);
    const auto irq = avr_io_getirq(simavr_avr.get(), irq_port, IOPORT_IRQ_PIN_ALL);

    std::unique_ptr<IrqCallback> irq_callback(
            new IrqCallback([=](const struct avr_irq_t *irq, const uint32_t value) -> void {
                callback(port, value);
            }));

    irq_callbacks.push_back(std::move(irq_callback));


    avr_irq_register_notify(irq, IrqCallbackInvoker, (void *) irq_callbacks.back().get());
}

void FlipDotTestSupportSimulator::AvrSimulator::RegisterPortPinChangeCallback(
        FlipDotTestSupportSimulator::Port port,
        FlipDotTestSupportSimulator::Pin pin,
        PinChangeCallback callback) {

    const auto irq_port = AVR_IOCTL_IOPORT_GETIRQ((char) port);
    const auto irq = avr_io_getirq(simavr_avr.get(), irq_port, (uint32_t) pin);


    std::unique_ptr<IrqCallback> irq_callback(
            new IrqCallback([=](const struct avr_irq_t *irq, const uint32_t value) -> void {
                callback(port, pin, value != 0);
            }));

    irq_callbacks.push_back(std::move(irq_callback));

    avr_irq_register_notify(irq, IrqCallbackInvoker, (void *) irq_callbacks.back().get());

}
