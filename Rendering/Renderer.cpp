#include "Renderer.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>

PropagationGraph* Renderer::graph        = nullptr;
int               Renderer::windowWidth  = 900;
int               Renderer::windowHeight = 600;

void Renderer::Init(int argc, char** argv, int width, int height, const char* title) {
    windowWidth  = width;
    windowHeight = height;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow(title);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);
}

void Renderer::SetGraph(PropagationGraph* g) {
    graph = g;
}

void Renderer::Run() {
    glutMainLoop();
}

void Renderer::display() {
    glViewport(0, 0, windowWidth, windowHeight);
    if (graph) graph->Draw();
    glutSwapBuffers();
}

void Renderer::keyboard(unsigned char key, int, int) {
    if (key == 'q' || key == 'Q') {
        std::exit(0);
    }
}

void Renderer::reshape(int w, int h) {
    windowWidth  = w;
    windowHeight = h;
    glViewport(0, 0, w, h);
}

void Renderer::timer(int) {
    if (graph) {
        for (int i = 0; i < 5; i++)
            graph->Advance();
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
