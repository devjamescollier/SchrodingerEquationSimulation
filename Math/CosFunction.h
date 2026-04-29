#pragma once

#include "Function.h"

class CosFunction : public Function {
public:
    CosFunction(double amplitude, double frequency, double phase);

    double Evaluate(double x) const override;

private:
    double amplitude;
    double frequency;
    double phase;
};
