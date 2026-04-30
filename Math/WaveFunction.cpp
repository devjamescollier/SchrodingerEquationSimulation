#include "WaveFunction.h"
#include "Complex.h"
#include <cmath>
#include <vector>
/*
double WaveFunction::Evaluate(double x) const {
  // The phase angle is irrelevant here since it does not change the squared
  // amplitude of the wave function
  //double x_amp = norm_factor * initial_profile>Evaluate(x);
  //return x_amp * x_amp;
  return 0.0;
}
*/

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

  std::vector<Complex> delta_psi(x_vals.size());
  // perform Runge Kutta
  for (int i = 0; i < x_vals.size(); i++) {
    Complex k1 = RungeKuttaRHS(i, potential, Complex(0, 0));
    Complex k2 = RungeKuttaRHS(i, potential, k1 * dt / 2);
    Complex k3 = RungeKuttaRHS(i, potential, k2 * dt / 2);
    Complex k4 = RungeKuttaRHS(i, potential, k3 * dt);
    delta_psi[i] = (dt / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
  }
  for (int i = 0; i < x_vals.size(); i++) {
    profile[i] += delta_psi[i];
  }
}

Complex WaveFunction::RungeKuttaRHS(int index, Function *potential,
                                    Complex k_val) {

  Complex wave_right, wave_left;
  if (index == 0) {
    wave_left = Complex(0, 0);
  } else
    wave_left = profile[index - 1];
  if (index == x_vals.size() - 1)
    wave_right = Complex(0, 0);
  else
    wave_right = profile[index + 1];

  double dx = x_vals[1] - x_vals[0]; // we can do this because we assert that
                                     // x_vals is equally spaced

  return Complex(0, 1) *
         ((wave_right + wave_left - 2 * (profile[index] + k_val)) /
              (2 * x_vals[index] * x_vals[index]) -
          potential->Evaluate(index) * (profile[index] + k_val));
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
