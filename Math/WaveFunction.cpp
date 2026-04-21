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

double WaveFunction::estimate_integral() {

  double sum = 0.0;
  // use the rectangle method to estimate the integral. We take the left point
  // of the interval as the height for the rectangle
  // We assume that the wave function is zero elsewhere
  for (int i = 0; i < x_vals.size() - 1; i++) {
    double dx = x_vals[i] - x_vals[i + 1];
    double height = profile[i].magnitudeSquared();
    sum += height * dx;
  }

  return sum;
}
