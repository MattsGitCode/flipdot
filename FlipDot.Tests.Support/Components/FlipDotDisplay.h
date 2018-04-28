#pragma once

#include <array>
#include <chrono>
#include <queue>

#include "ClockedCircuitComponent.h"
#include "ClockedPinBus.h"

namespace FlipDotTestSupport {
    namespace Peripherals {


        enum class FlipDotState {
            On, Off
        };

        template<size_t columnCount, size_t rowCount>
        class FlipDotDisplay;

        enum class FlipDotPolarity {
            None,
            RowPositive,
            ColumnPositive
        };

        class FlipDot {
        private:
            ClockedPin &columnPositivePin;
            ClockedPin &columnNegativePin;
            ClockedPin &rowPositivePin;
            ClockedPin &rowNegativePin;
            const size_t column;
            const size_t row;

            bool isOn = false;
            FlipDotPolarity currentPolarity = FlipDotPolarity::None;
            ClockCount currentPolarityFromClockCount = ClockCount::Zero;

        public:
            FlipDot(ClockedPin &columnPositivePin,
                      ClockedPin &columnNegativePin,
                      ClockedPin &rowPositivePin,
                      ClockedPin &rowNegativePin,size_t column,
                      size_t row)
                    : columnPositivePin(columnPositivePin),
                      columnNegativePin(columnNegativePin),
                      rowPositivePin(rowPositivePin),
                      rowNegativePin(rowNegativePin),
                      column(column),
                      row(row) {
            }

            template<size_t columnCount, size_t rowCount>
            void Tick(ClockCount clockCount, FlipDotDisplay<columnCount, rowCount> &flipDotDisplay);

        private:
            const FlipDotPolarity Polarity() {
                bool rowPositive = rowPositivePin.Value().IsHigh();
                bool rowNegative = rowNegativePin.Value().IsHigh();
                bool columnPositive = columnPositivePin.Value().IsHigh();
                bool columnNegative = columnNegativePin.Value().IsHigh();

                assert(!(rowPositive && rowNegative));
                assert(!(columnPositive && columnNegative));
                assert(!(rowPositive && columnPositive));
                assert(!(rowNegative && columnNegative));

                if (rowPositive && columnNegative) {
                    return FlipDotPolarity::RowPositive;
                } else if (rowNegative && columnPositive) {
                    return FlipDotPolarity::ColumnPositive;
                } else {
                    return FlipDotTestSupport::Peripherals::FlipDotPolarity::None;
                }
            }

            const FlipDotPolarity PolarityRequiredToTurnOn() const {
                if (column % 2 == 0) {
                    if (row % 2 == 0) {
                        return FlipDotPolarity::ColumnPositive;
                    } else {
                        return FlipDotPolarity::RowPositive;
                    }
                } else {
                    if (row % 2 == 0) {
                        return FlipDotPolarity::RowPositive;
                    } else {
                        return FlipDotPolarity::ColumnPositive;
                    }
                }
            }

            const FlipDotPolarity PolarityRequiredToTurnOff() const {
                if (PolarityRequiredToTurnOn() == FlipDotPolarity::ColumnPositive) {
                    return FlipDotPolarity::RowPositive;
                } else {
                    return FlipDotPolarity::ColumnPositive;
                }
            }
        };

        class ExpectedDelta {
        public:
            const size_t column;
            const size_t row;
            const FlipDotState state;

            ExpectedDelta(const size_t column, const size_t row, const FlipDotState state)
                    : column(column),
                      row(row),
                      state(state) {
            }
        };


        template<size_t columnCount, size_t rowCount>
        class FlipDotDisplay final : public DiscreteClockedCircuitComponent {
            friend class FlipDot;

        private:
            std::array<std::array<FlipDot, rowCount>, columnCount> dots;

            std::chrono::microseconds expectedDotPowerDuration = std::chrono::microseconds(0);
            bool shouldAssertDotPowerDuration = false;
            void AssertDotPowerDuration(std::chrono::microseconds powerAppliedFor) const;

            std::queue<ExpectedDelta> expectedDeltas;
            void AssertDotOperation(size_t column, size_t row, FlipDotState state);


            void Tick() final;

        public:
            explicit FlipDotDisplay()
                    : DiscreteClockedCircuitComponent("FlipDotDisplay"),
                      columnNegativeBus(*this, "ColumnNegative"),
                      columnPositiveBus(*this, "ColumnPositive"),
                      rowNegativeBus(*this, "RowNegative"),
                      rowPositiveBus(*this, "RowPositive"),
                      dots(MakeDotsArray(*this)) {}

            FlipDotDisplay(const FlipDotDisplay &) = delete;
            FlipDotDisplay(FlipDotDisplay &&) = delete;
            DiscreteClockedCircuitComponent &operator=(const DiscreteClockedCircuitComponent &component) = delete;
            DiscreteClockedCircuitComponent &operator=(DiscreteClockedCircuitComponent &&component) = delete;

            ClockedPinBus<30> columnNegativeBus;
            ClockedPinBus<30> columnPositiveBus;
            ClockedPinBus<8> rowNegativeBus;
            ClockedPinBus<8> rowPositiveBus;

            void ExpectDotPowerDuration(std::chrono::duration<int64_t, std::micro> duration);
            void AddExpectedDelta(size_t column, size_t row, FlipDotState state);
            void AssertAllDeltasMet();
        private:
            template<typename ColumnIndices = std::make_index_sequence<columnCount>>
            std::array<std::array<FlipDot, rowCount>, columnCount> MakeDotsArray(FlipDotDisplay &display);

            template<size_t ...columnIndex>
            std::array<std::array<FlipDot, rowCount>, columnCount>
            MakeDotsArray(FlipDotDisplay &display, std::index_sequence<columnIndex...>);

            template<typename RowIndices = std::make_index_sequence<rowCount>>
            std::array<FlipDot, rowCount> MakeDotsRowArray(FlipDotDisplay &display,
                                                             ClockedPin &columnPositivePin,
                                                             ClockedPin &columnNegativePin,
                                                             const size_t columnIndex) {
                return MakeDotsRowArray(display, columnPositivePin, columnNegativePin, columnIndex, RowIndices());
            }

            template<size_t ...rowIndex>
            std::array<FlipDot, rowCount>
            MakeDotsRowArray(FlipDotDisplay &display,
                             ClockedPin &columnPositivePin,
                             ClockedPin &columnNegativePin,
                             const size_t columnIndex,
                             std::index_sequence<rowIndex...>);


        };

        template<size_t columnCount, size_t rowCount>
        void FlipDotDisplay<columnCount, rowCount>::Tick() {
            for (auto &column : dots) {
                for (auto &dot : column) {
                    dot.Tick(CurrentClockCount(), *this);
                }
            }
        }

        template<size_t columnCount, size_t rowCount>
        void FlipDotDisplay<columnCount, rowCount>::ExpectDotPowerDuration(
                std::chrono::duration<int64_t, std::micro> duration) {
            expectedDotPowerDuration = duration;
            shouldAssertDotPowerDuration = true;
        }

        template<size_t columnCount, size_t rowCount>
        void FlipDotDisplay<columnCount, rowCount>::AssertDotPowerDuration(
                const std::chrono::microseconds powerAppliedFor) const {
            if (shouldAssertDotPowerDuration) {
                assert(powerAppliedFor == expectedDotPowerDuration);
            }
        }

        template<size_t columnCount, size_t rowCount>
        void FlipDotDisplay<columnCount, rowCount>::AddExpectedDelta(size_t column, size_t row, FlipDotState state) {
            expectedDeltas.emplace(column, row, state);
        }

        template<size_t columnCount, size_t rowCount>
        void FlipDotDisplay<columnCount, rowCount>::AssertDotOperation(size_t column, size_t row, FlipDotState state) {
            assert(expectedDeltas.size() != 0);

            auto expectedDelta = expectedDeltas.front();

            assert(expectedDelta.column == column && expectedDelta.row == row && expectedDelta.state == state);

            expectedDeltas.pop();
        }

        template<size_t columnCount, size_t rowCount>
        void FlipDotDisplay<columnCount, rowCount>::AssertAllDeltasMet() {
            assert(expectedDeltas.size() == 0);
        }

        template<size_t columnCount, size_t rowCount>
        template<typename ColumnIndices>
        std::array<std::array<FlipDot, rowCount>, columnCount>
        FlipDotDisplay<columnCount, rowCount>::MakeDotsArray(FlipDotDisplay &display) {
            return MakeDotsArray(display, ColumnIndices());
        }

        template<size_t columnCount, size_t rowCount>
        template<size_t... columnIndex>
        std::array<std::array<FlipDot, rowCount>, columnCount>
        FlipDotDisplay<columnCount, rowCount>::MakeDotsArray(FlipDotDisplay &display,
                                                             std::index_sequence<columnIndex...>) {
            return {
                    MakeDotsRowArray(display,
                                     display.columnPositiveBus.template PinAt<columnIndex>(),
                                     display.columnNegativeBus.template PinAt<columnIndex>(),
                                     columnIndex)...
            };
        }

        template<size_t columnCount, size_t rowCount>
        template<size_t... rowIndex>
        std::array<FlipDot, rowCount>
        FlipDotDisplay<columnCount, rowCount>::MakeDotsRowArray(FlipDotDisplay &display, ClockedPin &columnPositivePin,
                                                                ClockedPin &columnNegativePin, const size_t columnIndex,
                                                                std::index_sequence<rowIndex...>) {
            return {
                    FlipDot(columnPositivePin,
                            columnNegativePin,
                            display.rowPositiveBus.template PinAt<rowIndex>(),
                            display.rowNegativeBus.template PinAt<rowIndex>(),
                            columnIndex,
                            rowIndex)...
            };
        }
    }
}




template<size_t columnCount, size_t rowCount>
void FlipDotTestSupport::Peripherals::FlipDot::Tick(ClockCount clockCount,
                     FlipDotTestSupport::Peripherals::FlipDotDisplay<columnCount, rowCount> &flipDotDisplay) {
    auto polarity = Polarity();

    if (polarity == PolarityRequiredToTurnOn() && !isOn) {
        isOn = true;
        flipDotDisplay.AssertDotOperation(column, row, FlipDotTestSupport::Peripherals::FlipDotState::On);
    } else if (polarity == PolarityRequiredToTurnOff() && isOn) {
        isOn = false;
        flipDotDisplay.AssertDotOperation(column, row, FlipDotTestSupport::Peripherals::FlipDotState::Off);
    }

    if (polarity != currentPolarity) {
        if (polarity == FlipDotPolarity::None) {
            auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                    clockCount.TimeSince(currentPolarityFromClockCount));
            flipDotDisplay.AssertDotPowerDuration(microseconds);
        } else {
            currentPolarityFromClockCount = clockCount;
        }
        currentPolarity = polarity;
    }
}