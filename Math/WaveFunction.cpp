#include "WaveFunction.h"
#include "Complex.h"
#include <cmath>
#include <vector>

WaveFunction::WaveFunction(Function *initial_profile, double initial_phase,
                           std::vector<double> &x_arr)
    : x_vals(x_arr) {

  for (int i = 0; i < x_arr.size(); i++) {
    double magnitude = initial_profile->Evaluate(x_arr[i]);
    Complex profile_point(cos(initial_phase) * magnitude,
                          sin(initial_phase) * magnitude);
    profile.push_back(profile_point);
  }

  norm_factor = 1; // this way Evalute will just return amplitude squared

  norm_factor = 1 / estimate_integral();
}

const double WaveFunction::GetWavePDF(int index) {
  return norm_factor * profile[index].magnitudeSquared();
}

const std::vector<double> &WaveFunction::GetXVals() { return x_vals; }

void WaveFunction::RungeKuttaStep(double dt, Function *potential) {
  const int    n  = static_cast<int>(x_vals.size());
  const double dx = x_vals[1] - x_vals[0];

  std::vector<Complex> k1(n), k2(n), k3(n), k4(n), tmp(n);

  for (int i = 0; i < n; ++i)
    k1[i] = computeRHS(i, profile, potential, dx);

  for (int i = 0; i < n; ++i)
    tmp[i] = profile[i] + k1[i] * (dt / 2.0);
  for (int i = 0; i < n; ++i)
    k2[i] = computeRHS(i, tmp, potential, dx);

  for (int i = 0; i < n; ++i)
    tmp[i] = profile[i] + k2[i] * (dt / 2.0);
  for (int i = 0; i < n; ++i)
    k3[i] = computeRHS(i, tmp, potential, dx);

  for (int i = 0; i < n; ++i)
    tmp[i] = profile[i] + k3[i] * dt;
  for (int i = 0; i < n; ++i)
    k4[i] = computeRHS(i, tmp, potential, dx);

  for (int i = 0; i < n; ++i)
    profile[i] += (dt / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
}

Complex WaveFunction::computeRHS(int index, const std::vector<Complex> &psi,
                                  Function *potential, double dx) const {
  const Complex wave_left  = (index == 0)
                                 ? Complex(0, 0)
                                 : psi[index - 1];
  const Complex wave_right = (index == static_cast<int>(psi.size()) - 1)
                                 ? Complex(0, 0)
                                 : psi[index + 1];

  return Complex(0, 1) *
         ((wave_right + wave_left - 2.0 * psi[index]) / (2.0 * dx * dx) -
          potential->Evaluate(x_vals[index]) * psi[index]);
}

double WaveFunction::estimate_integral() {

  double sum = 0.0;
  // use the rectangle method to estimate the integral. We take the left point
  // of the interval as the height for the rectangle
  // We assume that the wave function is zero elsewhere
  for (int i = 0; i < x_vals.size() - 1; i++) {
    double dx = x_vals[i + 1] - x_vals[i];
    double height = profile[i].magnitudeSquared();
    sum += height * dx;
  }

  return sum;
}
