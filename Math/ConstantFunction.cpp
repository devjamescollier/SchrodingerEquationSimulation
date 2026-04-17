#include "ConstantFunction.h"

ConstantFunction::ConstantFunction(double constant) : constant(constant) {}

double ConstantFunction::Evaluate(double x) const {
    return constant;
}

double ConstantFunction::EvaluateDerivative(double x) const {
    return 0.0;
}

Function* ConstantFunction::Clone() const {
    return new ConstantFunction(constant);
}

bool ConstantFunction::IsConstant() const {
    return true;
}
