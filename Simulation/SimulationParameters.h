#pragma once

struct SimulationParameters {
    int    nPoints;
    double xMin;
    double xMax;
    double dt;

    SimulationParameters(int nPoints, double xMin, double xMax, double dt)
        : nPoints(nPoints), xMin(xMin), xMax(xMax), dt(dt) {}

    double dx() const { return (xMax - xMin) / (nPoints - 1); }
};
