//
//  viewer.hpp
//  Thanda

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "../camera/camera.hpp"
#include "math.h"

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
//    int runLoop(); // Also clean up

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//    static void adjustPhi(Camera c, float p);
//    void adjustTheta(float t);

    GLFWwindow* window;
    Camera* camera;
};
