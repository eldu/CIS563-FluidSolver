//#ifndef FS_CONTROLS_H
//#define FS_CONTROLS_H


/////// References
/////// THE IDEA: Callback functions require things to either be passed in or things
/////// to be static in order to have any effect. I was not able to pass in
/////// the camera object into the callback function to have the camera parameters
/////// changed. Therefore, Austin suggested to create a singleton static class to
/////// handle events
///////
/////// Sources of information
/////// Stackoverflow: How to declare a static instance of a class in C++?
/////// http://stackoverflow.com/questions/19130021/
///////

////// Include GLFW
//#include <GLFW/glfw3.h>
//////extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

////// Include GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

//using namespace glm;

////void computeMatricesFromInputs(GLFWwindow *window);
////glm::mat4 getViewMatrix();
////glm::mat4 getProjectionMatrix();

//class Controls {
//public:
//    static Controls &controlsInstance() {
//        static Controls controls;
//        return controls;
//    }

//    void computeMatricesFromInputs(GLFWwindow* window);

//private:
//    Controls();

//    double mouseX;
//    double mouseY;


//};


















/////**
////  * Currently just the controls.hpp and controls.cpp as seen in the OpenGL tutorials
////  * from www.opengl-tutorial.org
////  *
////  * Have not yet actually intergrated camera controls.
////  */



//////// Include GLFW
//////#include <GLFW/glfw3.h>
////////extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

//////// Include GLM
//////#include <glm/glm.hpp>
//////#include <glm/gtc/matrix_transform.hpp>
//////using namespace glm;

//////void computeMatricesFromInputs(GLFWwindow *window);
//////glm::mat4 getViewMatrix();
//////glm::mat4 getProjectionMatrix();

//#endif
