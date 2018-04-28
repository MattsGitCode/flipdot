#include "RegressionTest.h"

#include <iostream>

#include "FlipDotCircuit.h"

namespace AvrSim = FlipDotTestSupportSimulator;
namespace Peripherals = FlipDotTestSupport::Peripherals;

RegressionTest::RegressionTest() {}

RegressionTest::~RegressionTest() {}

void RegressionTest::SetUp() {}

void RegressionTest::TearDown() {}

void TestAllDotsOnAndOff(std::string elfPath) {
    FlipDotCircuit circuit(elfPath);

    circuit.FlipDotDisplay().ExpectDotPowerDuration(std::chrono::microseconds(500));

    auto expectAll = [&](Peripherals::FlipDotState state) {
        for (int column = 0; column < 30; ++column) {
            for (int row = 0; row < 8; ++row) {
                circuit.FlipDotDisplay().AddExpectedDelta(column, row, state);
            }
        }
    };

    expectAll(Peripherals::FlipDotState::On);
    expectAll(Peripherals::FlipDotState::Off);

    circuit.Run();

    circuit.FlipDotDisplay().AssertAllDeltasMet();
}

TEST_F(RegressionTest, ProofOfConceptBinaryTurnsAllOnAndOff) {
    TestAllDotsOnAndOff("ProofOfConceptBinary/FlipDot30x8.elf");
}