#pragma once

#include <iostream>
#include <bitset>
#include <vector>

#include "ClockedCircuitComponent.h"
#include "ClockedPin.h"
#include "ClockedPinBus.h"


namespace FlipDotTestSupport {

    class SerialToParallel_74HC595_Array;

    class SerialToParallel_74HC595 final : public DiscreteClockedCircuitComponent {

    private:
        std::bitset<8> latchedValue;
        std::bitset<8> unlatchedValue;

        void Tick() final;


    public:
        explicit SerialToParallel_74HC595(std::string name)
                : DiscreteClockedCircuitComponent(std::move(name)),
                  DataPin(*this, "Data"),
                  ClockPin(*this, "Clock"),
                  LatchPin(*this, "Latch"),
                  OutputEnablePin(*this, "OutputEnable"),
                  SerialOutPin(*this, "SerialOut"),
                  OutputPinBus(*this, "Output") {

        }

        SerialToParallel_74HC595(const SerialToParallel_74HC595 &) = delete;
        SerialToParallel_74HC595(SerialToParallel_74HC595 &&) noexcept = delete;
        void operator=(const SerialToParallel_74HC595 &) = delete;


        ClockedPin DataPin;
        ClockedPin ClockPin;
        ClockedPin LatchPin;
        ClockedPin OutputEnablePin;

        ClockedPin SerialOutPin;

        ClockedPinBus<8> OutputPinBus;


        class CascadeBuilder {
            friend class SerialToParallel_74HC595;

        private:
            std::vector<std::reference_wrapper<SerialToParallel_74HC595>> cascaded;

            explicit CascadeBuilder(SerialToParallel_74HC595 &current);

        public:
            struct CascadeBuilder AndThenTo(SerialToParallel_74HC595 &next);
        };


        struct CascadeBuilder CascadeTo(SerialToParallel_74HC595 &next);
    };
}


