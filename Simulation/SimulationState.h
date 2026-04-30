#pragma once

#include "../Math/Function.h"
#include "../Math/WaveFunction.h"

class SimulationState {
public:
    SimulationState(WaveFunction* waveFunction, Function* potential, double dt);

    void Advance();
    void Reset();

    double        GetTime()          const;
    int           GetStepCount()     const;
    WaveFunction* GetWaveFunction()  const;
    Function*     GetPotential()     const;
    double        GetDt()            const;

private:
    WaveFunction* waveFunction;
    Function*     potential;
    double        dt;
    double        time;
    int           stepCount;
};
