#include "SchrodingerSolver.h"
#include <cmath>

SchrodingerSolver::SchrodingerSolver(SimulationState* state,
                                     const SimulationParameters& params)
    : state(state), params(params) {}

void SchrodingerSolver::Step() {
    state->Advance();
}

void SchrodingerSolver::StepN(int n) {
    for (int i = 0; i < n; ++i)
        state->Advance();
}

void SchrodingerSolver::StepTo(double targetTime) {
    const double dt = state->GetDt();
    while (state->GetTime() + 0.5 * dt < targetTime)
        state->Advance();
}

double SchrodingerSolver::ComputeNorm() const {
    WaveFunction* wf  = state->GetWaveFunction();
    const double  dx  = params.dx();
    double        sum = 0.0;
    for (int i = 0; i < params.nPoints; ++i)
        sum += wf->GetWavePDF(i) * dx;
    return sum;
}

bool SchrodingerSolver::IsStable() const {
    WaveFunction* wf = state->GetWaveFunction();
    for (int i = 0; i < params.nPoints; ++i) {
        if (!std::isfinite(wf->GetWavePDF(i)))
            return false;
    }
    return true;
}
