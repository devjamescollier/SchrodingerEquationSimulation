#pragma once

class Function {
public:
    virtual ~Function() = default;

    virtual double Evaluate(double x) const = 0;
};
