# Flip-Dot

This flip-dot project currently contains a minimal binary produced by the https://github.com/MattsGitCode/FlipDotTetris proof-of-concept along with a single test that uses a virtual circuit built around https://github.com/buserror/simavr/.

The intention is to implement a more robust and general purpose flip-dot controller, using this existing test to prove that it won't immediately fry the circuit (by enabling the wrong driver lines), and then building upon the tests as functionality is added.