#include "Function.h"

    constexpr double h = 1e-7;
    return (Evaluate(x + h) - Evaluate(x - h)) / (2.0 * h);
}

bool Function::IsConstant() const {
        return false;
    }