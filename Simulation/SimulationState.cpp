#include "SimulationState.h"

SimulationState::SimulationState(WaveFunction* waveFunction, Function* potential, double dt)
    : waveFunction(waveFunction), potential(potential), dt(dt), time(0.0), stepCount(0) {}

void SimulationState::Advance() {
    waveFunction->RungeKuttaStep(dt, potential);
    time += dt;
    ++stepCount;
}

void SimulationState::Reset() {
    time      = 0.0;
    stepCount = 0;
}

double        SimulationState::GetTime()         const { return time; }
int           SimulationState::GetStepCount()    const { return stepCount; }
WaveFunction* SimulationState::GetWaveFunction() const { return waveFunction; }
Function*     SimulationState::GetPotential()    const { return potential; }
double        SimulationState::GetDt()           const { return dt; }
