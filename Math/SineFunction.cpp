#include "SineFunction.h"
#include <cmath>

SineFunction::SineFunction(double amplitude, double frequency, double phase)
    : amplitude(amplitude), frequency(frequency), phase(phase) {}

double SineFunction::Evaluate(double x) const {
    return amplitude * std::sin(frequency * x + phase);
}
