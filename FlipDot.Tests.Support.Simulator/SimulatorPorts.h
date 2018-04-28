#pragma once

#include <iostream>

namespace FlipDotTestSupportSimulator {

    enum class Port : char {
        A = 'A', B, C, D
    };

    inline std::ostream &operator<<(std::ostream &out, const Port &port) {
        return out << (char)port;
    }


    enum class Pin {
        _0 = 0, _1, _2, _3, _4, _5, _6, _7, _8
    };

    inline std::ostream &operator<<(std::ostream &out, const Pin &pin) {
        return out << (int)pin;
    }
}