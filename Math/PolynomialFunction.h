#pragma once

#include "Function.h"
#include <vector>

class PolynomialFunction : public Function {
public:
    explicit PolynomialFunction(const std::vector<double>& coefficients);
    explicit PolynomialFunction(const PolynomialFunction& func);

    double Evaluate(double x) const override;

private:
    std::vector<double> coefficients;
};
