#pragma once

#include "Function.h"

class ConstantFunction : public Function {
public:
    explicit ConstantFunction(double constant);

    double Evaluate(double x) const override;
    double EvaluateDerivative(double x) const override;
    Function* Clone() const override;
    bool IsConstant() const override;

private:
    double constant;
};
