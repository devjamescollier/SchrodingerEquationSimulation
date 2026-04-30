#pragma once

#include "SimulationParameters.h"
#include "SimulationState.h"

class SchrodingerSolver {
public:
    SchrodingerSolver(SimulationState* state, const SimulationParameters& params);

    void Step();
    void StepN(int n);
    void StepTo(double targetTime);

    double ComputeNorm() const;
    bool   IsStable()    const;

private:
    SimulationState*     state;
    SimulationParameters params;
};
