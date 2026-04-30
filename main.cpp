#include "Math/ConstantFunction.h"
#include "Math/CosFunction.h"
#include "Math/PolynomialFunction.h"
#include "Math/SineFunction.h"
#include "Math/WaveFunction.h"
#include "Rendering/PropagationGraph.h"
#include "Rendering/Renderer.h"

#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

namespace {
double readDouble(const std::string &label) {
  double value = 0.0;
  while (true) {
    std::cout << label;
    if (std::cin >> value)
      return value;
    std::cout << "Invalid number. Try again.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

int readInt(const std::string &label, int minValue, int maxValue) {
  int value = minValue;
  while (true) {
    std::cout << label;
    if (std::cin >> value && value >= minValue && value <= maxValue)
      return value;
    std::cout << "Invalid choice. Enter a number from " << minValue << " to "
              << maxValue << ".\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}

std::unique_ptr<Function> buildFunctionFromMenu(const std::string &title) {
  std::cout << "\nSelect " << title << ":\n";
  std::cout << "  1) Sine\n";
  std::cout << "  2) Cosine\n";
  std::cout << "  3) Constant\n";
  std::cout << "  4) Polynomial\n";
  const int choice = readInt("Choice [1-4]: ", 1, 4);

  if (choice == 1) {
    const double amplitude = readDouble("  amplitude: ");
    const double frequency = readDouble("  frequency: ");
    const double phase = readDouble("  phase: ");
    return std::make_unique<SineFunction>(amplitude, frequency, phase);
  }
  if (choice == 2) {
    const double amplitude = readDouble("  amplitude: ");
    const double frequency = readDouble("  frequency: ");
    const double phase = readDouble("  phase: ");
    return std::make_unique<CosFunction>(amplitude, frequency, phase);
  }
  if (choice == 3) {
    const double constant = readDouble("  constant value: ");
    return std::make_unique<ConstantFunction>(constant);
  }

  const int degree = readInt("  polynomial degree (>= 0): ", 0, 20);
  std::vector<double> coefficients(static_cast<size_t>(degree) + 1);
  std::cout << "  Enter coefficients c0..c" << degree
            << " for c0 + c1*x + ...\n";
  for (int i = 0; i <= degree; ++i) {
    coefficients[static_cast<size_t>(i)] =
        readDouble("    c" + std::to_string(i) + ": ");
  }
  return std::make_unique<PolynomialFunction>(coefficients);
}
}

int main(int argc, char **argv) {
  const int nPoints = 800;
  const double xMin = -10.0;
  const double xMax = 10.0;
  const double dt = 0.0005;

  std::vector<double> xVals;
  xVals.reserve(nPoints);
  const double dx = (xMax - xMin) / (nPoints - 1);
  for (int i = 0; i < nPoints; ++i) {
    xVals.push_back(xMin + i * dx);
  }

  std::cout << "1D Schrodinger Simulation Setup\n";
  auto initialProfile = buildFunctionFromMenu("initial profile");
  auto potential = buildFunctionFromMenu("potential");
  const double initialPhase = readDouble("\nInitial global phase (radians): ");

  WaveFunction waveFunction(initialProfile.get(), initialPhase, xVals);
  PropagationGraph graph(&waveFunction, potential.get(), dt);

  std::cout << "Starting Schrodinger simulation with " << nPoints
            << " grid points." << std::endl;

  Renderer::Init(argc, argv, 1000, 700, "1D Schrodinger Propagation");
  Renderer::SetGraph(&graph);
  Renderer::Run();

  return 0;
}
