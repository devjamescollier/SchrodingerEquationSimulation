#pragma once

class Function {
public:
    virtual ~Function() = default;

    virtual double Evaluate(double x) const = 0;

    virtual double EvaluateDerivative(double x) const;

    virtual Function* Clone() const = 0;

    virtual bool IsConstant() const;
};