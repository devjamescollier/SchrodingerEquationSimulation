#pragma once

#include <ostream>

class Complex {
public:
    double real;
    double imag;

    Complex(double real = 0.0, double imag = 0.0);

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(const Complex& other) const;
    Complex operator/(const Complex& other) const;

    Complex& operator+=(const Complex& other);
    Complex& operator-=(const Complex& other);
    Complex& operator*=(const Complex& other);
    Complex& operator/=(const Complex& other);

    Complex operator*(double scalar) const;
    Complex operator/(double scalar) const;

    bool operator==(const Complex& other) const;
    bool operator!=(const Complex& other) const;

    Complex conjugate() const;
    double magnitude() const;
    double magnitudeSquared() const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};

Complex operator*(double scalar, const Complex& c);
