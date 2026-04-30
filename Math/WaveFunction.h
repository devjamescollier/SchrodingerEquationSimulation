#pragma once

#include "Complex.h"
#include "Function.h"
#include <vector>

class WaveFunction {
public:
  WaveFunction(Function *initial_profile, double init_phase,
               std::vector<double> &x_arr);

  const std::vector<double> &GetXVals();
  const double GetWavePDF(int index);

  Complex EvaluateComplex(
      double x); // returns the complex value of the wave function at a point
  void RungeKuttaStep(double dt, Function *potential);

private:
  double norm_factor; // use this to make sure the integral of the pdf is 1
  std::vector<Complex> profile;
  std::vector<double> x_vals;
  double estimate_integral(); // integral of the UNORMALIZED wavefunction
  Complex computeRHS(int index, const std::vector<Complex> &psi,
                     Function *potential, double dx) const;
};
