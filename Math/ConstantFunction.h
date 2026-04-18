#pragma once

#include "Function.h"

class ConstantFunction : public Function {
public:
    explicit ConstantFunction(double constant);
    explicit ConstantFunction(const ConstantFunction& func);

    double Evaluate(double x) const override;

private:
    double constant;
};
