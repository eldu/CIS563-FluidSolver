// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "shaders/shader.hpp"
#include "camera/camera.hpp"
#include "viewer/viewer.hpp"

int main( void )
{
    Viewer* viewer = new Viewer(1024, 768);
    viewer->initialize();
    viewer->run();
    return 0;
}
