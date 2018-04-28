configure_file(
        "ProofOfConceptBinary/FlipDot30x8.elf"
        "${CMAKE_CURRENT_BINARY_DIR}/ProofOfConceptBinary/FlipDot30x8.elf"
        COPYONLY)

add_executable(FlipDot.Core.Tests
        main.cpp
        FlipDotCircuit.cpp
        RegressionTest.cpp)


target_link_libraries(FlipDot.Core.Tests
        FlipDot.Core
        FlipDot.Tests.Support.Simulator
        FlipDot.Tests.Support
        GoogleTest)


add_test(NAME MyTests
        COMMAND FlipDot.Core.Tests)