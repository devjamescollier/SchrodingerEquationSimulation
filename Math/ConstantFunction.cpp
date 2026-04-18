#include "ConstantFunction.h"

ConstantFunction::ConstantFunction(double constant) : constant(constant) {}
ConstantFunction::ConstantFunction(const ConstantFunction& func) : constant(func.constant) {}

double ConstantFunction::Evaluate(double x) const { return constant; }
