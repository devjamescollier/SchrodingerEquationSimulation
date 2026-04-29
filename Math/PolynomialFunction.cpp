#include "PolynomialFunction.h"

PolynomialFunction::PolynomialFunction(const std::vector<double>& coefficients)
    : coefficients(coefficients) {}

PolynomialFunction::PolynomialFunction(const PolynomialFunction& func)
    : coefficients(func.coefficients) {}

double PolynomialFunction::Evaluate(double x) const {
    if (coefficients.empty()) return 0.0;
    double result = coefficients.back();
    for (int i = static_cast<int>(coefficients.size()) - 2; i >= 0; --i)
        result = result * x + coefficients[i];

    return result;
}
