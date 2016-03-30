//
//  viewer.hpp
//  Thanda

#ifndef FS_VIEWER_H
#define FS_VIEWER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "../camera/camera.hpp"
#include "math.h"
#include "../la.hpp"

#include "../shaders/shader.hpp"
#include "../camera/controls.hpp"
#include "../geom/cube.hpp"
#include "../geom/triangle.hpp"
#include "../scene/jsonreader.hpp"

#include "../fluidSolver/flipSolver.hpp"

class Viewer {
private:
    int x;
    int y;
    float DEG2RAD = M_PI / 180.0;
public:
    Viewer(int width, int height);
    Viewer();
    ~Viewer();

    int initialize();
    int run();
//    int runLoop(); // Also clean up

//    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//    static void adjustPhi(Camera c, float p);
//    void adjustTheta(float t);

    GLuint programID;
    GLuint matrixID;
    GLuint VertexArrayID;

    GLFWwindow* window;
    Camera* camera;

    // cleanup
    FLIPSolver* fs;
};

#endif
