#pragma once

#include <functional>

#include <AvrSimulator.h>

#include "ClockedCircuitComponent.h"
#include "ClockedPin.h"

namespace FlipDotTestSupport {

    class Atmega88PA final : public PrimaryClockedCircuitComponent {
    private:
        FlipDotTestSupportSimulator::AvrSimulator avrSimulator;
        ClockCount lastProcessed = ClockCount::Zero;
    public:
        void EnsureTicksMatch(ClockedCircuitComponent &otherComponent) override;
    public:
        const ClockCount Tick() override;

    public:
        void Tick(ClockCount count) override;
        explicit Atmega88PA(std::string elf_path, size_t frequencyInHertz);
        Atmega88PA(const Atmega88PA &) = delete;
        Atmega88PA(Atmega88PA &&) = delete;
        Atmega88PA &operator=(const Atmega88PA &) = delete;
        Atmega88PA &operator=(Atmega88PA &) = delete;

        inline bool IsDone() {
            return avrSimulator.IsDone();
        }

        class Port {
        private:
            void WireupOutputPin(ClockedPin &outputPin,
                                 Atmega88PA &atmega88PA,
                                 FlipDotTestSupportSimulator::Port,
                                 FlipDotTestSupportSimulator::Pin);
        public:
            ClockedPin Pin0;
            ClockedPin Pin1;
            ClockedPin Pin2;
            ClockedPin Pin3;
            ClockedPin Pin4;
            ClockedPin Pin5;
            ClockedPin Pin6;
            ClockedPin Pin7;


            Port(Atmega88PA &atmega88PA, FlipDotTestSupportSimulator::Port);
        };

        struct Port PortD;
    };
}


