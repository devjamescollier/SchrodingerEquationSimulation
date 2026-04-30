#pragma once

#include "PropagationGraph.h"

class Renderer {
public:
    static void Init(int argc, char** argv, int width, int height, const char* title);
    static void SetGraph(PropagationGraph* graph);
    static void Run();

private:
    static PropagationGraph* graph;
    static int               windowWidth;
    static int               windowHeight;

    static void display();
    static void keyboard(unsigned char key, int x, int y);
    static void reshape(int w, int h);
    static void timer(int value);
};
