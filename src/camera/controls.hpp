#pragma once;


/**
  * Currently just the controls.hpp and controls.cpp as seen in the OpenGL tutorials
  * from www.opengl-tutorial.org
  *
  * Have not yet actually intergrated camera controls.
  */



// Include GLFW
#include <GLFW/glfw3.h>
//extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

void computeMatricesFromInputs(GLFWwindow *window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
