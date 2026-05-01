#include "PropagationGraph.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cmath>
#include <limits>
#include <sstream>
#include <iomanip>
#include <string>

void PropagationGraph::hsvToRgb(float h, float& r, float& g, float& b) {
    h = std::fmod(h, 1.0f);
    if (h < 0.0f) h += 1.0f;
    int   i = static_cast<int>(h * 6.0f);
    float f = h * 6.0f - i;
    float q = 1.0f - f;
    switch (i % 6) {
        case 0: r = 1.0f; g = f;    b = 0.0f; break;
        case 1: r = q;    g = 1.0f; b = 0.0f; break;
        case 2: r = 0.0f; g = 1.0f; b = f;    break;
        case 3: r = 0.0f; g = q;    b = 1.0f; break;
        case 4: r = f;    g = 0.0f; b = 1.0f; break;
        case 5: r = 1.0f; g = 0.0f; b = q;    break;
        default: r = g = b = 0.0f;
    }
}

PropagationGraph::PropagationGraph(WaveFunction* waveFunc,
                                   Function*     potential,
                                   double        dt)
    : waveFunction(waveFunc), potential(potential), dt(dt), time(0.0) {}

void PropagationGraph::Advance() {
    waveFunction->RungeKuttaStep(dt, potential);
    time += dt;
}

double PropagationGraph::GetTime() const { return time; }

void PropagationGraph::Draw() const {
    const std::vector<double>& xVals = waveFunction->GetXVals();
    int n = static_cast<int>(xVals.size());
    if (n < 2) return;

    double xMin = xVals.front();
    double xMax = xVals.back();

    double maxPDF = 0.0;
    for (int i = 0; i < n; i++) {
        double pdf = waveFunction->GetWavePDF(i);
        if (!std::isfinite(pdf)) continue;
        double v = std::abs(pdf);
        if (v > maxPDF) maxPDF = v;
    }
    if (maxPDF < 1e-12) maxPDF = 1.0;

    double yMax   = maxPDF * 1.35;
    double xRange = xMax - xMin;
    double xPad   = xRange * 0.07;
    double yPad   = yMax  * 0.14;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(xMin - xPad * 3.2, xMax + xPad, -yPad * 1.8, yMax + yPad, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    drawFilledCurve(xMin, xMax, yMax);
    drawPotential(xMin, xMax, yMax);
    drawAxes(xMin, xMax, yMax);
}

void PropagationGraph::drawFilledCurve(double xMin, double xMax, double yMax) const {
    const std::vector<double>& xVals = waveFunction->GetXVals();
    int n = static_cast<int>(xVals.size());

    glBegin(GL_QUADS);
    for (int i = 0; i < n - 1; i++) {
        double x0 = xVals[i];
        double x1 = xVals[i + 1];

        double rawPdf0 = waveFunction->GetWavePDF(i);
        double rawPdf1 = waveFunction->GetWavePDF(i + 1);
        double pdf0 = (std::isfinite(rawPdf0)) ? std::abs(rawPdf0) : 0.0;
        double pdf1 = (std::isfinite(rawPdf1)) ? std::abs(rawPdf1) : 0.0;

        float hue0 = static_cast<float>((x0 - xMin) / (xMax - xMin)) * 0.82f;
        float hue1 = static_cast<float>((x1 - xMin) / (xMax - xMin)) * 0.82f;
        float r0, g0, b0, r1, g1, b1;
        hsvToRgb(hue0, r0, g0, b0);
        hsvToRgb(hue1, r1, g1, b1);

        const float dimFactor = 0.12f;

        glColor3f(r0, g0, b0);
        glVertex2d(x0, pdf0);
        glColor3f(r1, g1, b1);
        glVertex2d(x1, pdf1);
        glColor3f(r1 * dimFactor, g1 * dimFactor, b1 * dimFactor);
        glVertex2d(x1, 0.0);
        glColor3f(r0 * dimFactor, g0 * dimFactor, b0 * dimFactor);
        glVertex2d(x0, 0.0);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; i++) {
        double raw = waveFunction->GetWavePDF(i);
        double pdf = (std::isfinite(raw)) ? std::abs(raw) : 0.0;
        glVertex2d(xVals[i], pdf);
    }
    glEnd();
    glLineWidth(1.0f);
}

void PropagationGraph::drawPotential(double xMin, double xMax, double yMax) const {
    if (!potential) return;
    const std::vector<double>& xVals = waveFunction->GetXVals();
    const int n = static_cast<int>(xVals.size());

    double vMin = std::numeric_limits<double>::max();
    double vMax = std::numeric_limits<double>::lowest();
    for (int i = 0; i < n; ++i) {
        const double v = potential->Evaluate(xVals[i]);
        if (std::isfinite(v)) {
            if (v < vMin) vMin = v;
            if (v > vMax) vMax = v;
        }
    }
    const double vRange = vMax - vMin;
    if (vRange < 1e-12) return; // constant potential — nothing useful to show

    const double displayHeight = 0.28 * yMax;
    const double xRange        = xMax - xMin;

    glColor4f(1.0f, 0.55f, 0.0f, 0.85f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < n; ++i) {
        const double v = potential->Evaluate(xVals[i]);
        if (!std::isfinite(v)) continue;
        glVertex2d(xVals[i], (v - vMin) / vRange * displayHeight);
    }
    glEnd();
    glLineWidth(1.0f);

    drawText(xMin - xRange * 0.19, displayHeight * 0.5, "V(x)");
    glColor3f(1.0f, 1.0f, 1.0f);
}

void PropagationGraph::drawAxes(double xMin, double xMax, double yMax) const {
    const double xRange = xMax - xMin;
    const double tickH  = yMax  * 0.025;
    const double tickW  = xRange * 0.008;

    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);

    glBegin(GL_LINES);
    glVertex2d(xMin, 0.0); glVertex2d(xMax, 0.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2d(xMin, 0.0); glVertex2d(xMin, yMax);
    glEnd();

    glLineWidth(1.0f);

    double xSteps[] = {1.0, 2.0, 2.5, 5.0, 10.0};
    double xStep = 2.5;
    for (double s : xSteps) {
        int count = static_cast<int>(xRange / s) + 1;
        if (count >= 7 && count <= 10) { xStep = s; break; }
    }

    double xStart = std::ceil(xMin / xStep) * xStep;
    for (double x = xStart; x <= xMax + 1e-9; x += xStep) {
        glBegin(GL_LINES);
        glVertex2d(x, 0.0); glVertex2d(x, -tickH);
        glEnd();

        std::ostringstream oss;
        if (std::abs(x - std::round(x)) < 0.001)
            oss << std::fixed << std::setprecision(0) << std::round(x);
        else
            oss << std::fixed << std::setprecision(1) << x;
        std::string label = oss.str();

        double charWorldWidth = xRange * (7.0 / 850.0);
        double labelOffsetX   = charWorldWidth * label.size() * 0.5;
        drawText(x - labelOffsetX, -tickH * 3.5, label.c_str());
    }

    double xMid = (xMin + xMax) * 0.5;
    drawText(xMid - xRange * 0.01, -tickH * 7.0, "x");

    int    nYTicks = 5;
    double yStep   = yMax / nYTicks;

    for (int k = 0; k <= nYTicks; k++) {
        double y = k * yStep;
        glBegin(GL_LINES);
        glVertex2d(xMin, y); glVertex2d(xMin - tickW, y);
        glEnd();

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << y;
        drawText(xMin - tickW * 9.0, y - tickH * 0.5, oss.str().c_str());
    }

    drawText(xMin - xRange * 0.19, yMax * 0.45, "|psi(x)|^2");

    std::ostringstream toss;
    toss << "t = " << std::fixed << std::setprecision(3) << time << " a.u.";
    drawText(xMin + xRange * 0.02, yMax * 0.91, toss.str().c_str());
}

void PropagationGraph::drawText(double x, double y, const char* text) const {
    glRasterPos2d(x, y);
    for (const char* c = text; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
}
