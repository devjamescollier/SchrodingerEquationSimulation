#pragma once

#include "../Math/WaveFunction.h"
#include "../Math/Function.h"

class PropagationGraph {
public:
    PropagationGraph(WaveFunction* waveFunc, Function* potential, double dt);

    void   Advance();
    void   Draw() const;
    double GetTime() const;

private:
    WaveFunction* waveFunction;
    Function*     potential;
    double        dt;
    double        time;

    void drawFilledCurve(double xMin, double xMax, double yMax) const;
    void drawPotential(double xMin, double xMax, double yMax) const;
    void drawAxes(double xMin, double xMax, double yMax) const;
    void drawText(double x, double y, const char* text) const;

    static void hsvToRgb(float h, float& r, float& g, float& b);
};
