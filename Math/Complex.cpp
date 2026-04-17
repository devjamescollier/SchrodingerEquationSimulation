#include "Complex.h"
#include <cmath>
#include <stdexcept>

Complex::Complex(double real, double imag) : real(real), imag(imag) {}

Complex Complex::operator+(const Complex& other) const {
    return Complex(real + other.real, imag + other.imag);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real - other.real, imag - other.imag);
}

Complex Complex::operator*(const Complex& other) const {
    return Complex(
        real * other.real - imag * other.imag,
        real * other.imag + imag * other.real
    );
}

Complex Complex::operator/(const Complex& other) const {
    double denom = other.real * other.real + other.imag * other.imag;
    if (denom == 0.0)
        throw std::domain_error("Complex division by zero");
    return Complex(
        (real * other.real + imag * other.imag) / denom,
        (imag * other.real - real * other.imag) / denom
    );
}

Complex& Complex::operator+=(const Complex& other) {
    real += other.real;
    imag += other.imag;
    return *this;
}

Complex& Complex::operator-=(const Complex& other) {
    real -= other.real;
    imag -= other.imag;
    return *this;
}

Complex& Complex::operator*=(const Complex& other) {
    *this = *this * other;
    return *this;
}

Complex& Complex::operator/=(const Complex& other) {
    *this = *this / other;
    return *this;
}

Complex Complex::operator*(double scalar) const {
    return Complex(real * scalar, imag * scalar);
}

Complex Complex::operator/(double scalar) const {
    if (scalar == 0.0)
        throw std::domain_error("Complex division by zero scalar");
    return Complex(real / scalar, imag / scalar);
}

bool Complex::operator==(const Complex& other) const {
    return real == other.real && imag == other.imag;
}

bool Complex::operator!=(const Complex& other) const {
    return !(*this == other);
}

Complex Complex::conjugate() const {
    return Complex(real, -imag);
}

double Complex::magnitude() const {
    return std::sqrt(real * real + imag * imag);
}

double Complex::magnitudeSquared() const {
    return real * real + imag * imag;
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real;
    if (c.imag >= 0.0)
        os << "+" << c.imag << "i";
    else
        os << c.imag << "i";
    return os;
}

Complex operator*(double scalar, const Complex& c) {
    return c * scalar;
}
