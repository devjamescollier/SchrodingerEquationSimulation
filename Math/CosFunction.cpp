#include "CosFunction.h"
#include <cmath>

CosFunction::CosFunction(double amplitude, double frequency, double phase)
    : amplitude(amplitude), frequency(frequency), phase(phase) {}

double CosFunction::Evaluate(double x) const {
    return amplitude * std::cos(frequency * x + phase);
}
