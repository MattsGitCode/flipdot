#pragma once

#include <iostream>
#include <cassert>

class ClockedPinValue {
private :
    bool isHigh;
    bool isLow;
    bool isRisingEdge;
    bool isFallingEdge;

    explicit ClockedPinValue(bool isHigh)
            : isHigh(isHigh),
              isLow(!isHigh),
              isRisingEdge(false),
              isFallingEdge(false) {}

public:

    ClockedPinValue(ClockedPinValue previousValue, ClockedPinValue newValue)
            : isHigh(newValue.IsHigh()),
              isLow(newValue.IsLow()),
              isRisingEdge(previousValue.IsLow() && newValue.IsHigh()),
              isFallingEdge(previousValue.IsHigh() && newValue.IsLow()) {}


    inline const static ClockedPinValue High() {
        return ClockedPinValue(true);
    }

    inline const static ClockedPinValue Low() {
        return ClockedPinValue(false);
    }

    inline bool IsHigh() const {
        return isHigh;
    }

    inline bool IsLow() const {
        return isLow;
    };

    inline bool IsRisingEdge() const {
        return isRisingEdge;
    }

    inline bool IsFallingEdge() const {
        return isFallingEdge;
    }
};

inline std::ostream &operator<<(std::ostream &out, const ClockedPinValue &pinValue) {
    assert(pinValue.IsHigh() || pinValue.IsLow());

    if (pinValue.IsRisingEdge()) {
        return out << "RisingEdge";
    } else if (pinValue.IsFallingEdge()) {
        return out << "FallingEdge";
    } else if (pinValue.IsHigh()) {
        return out << "High";
    } else if (pinValue.IsLow()) {
        return out << "Low";
    }
}
