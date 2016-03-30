 //
//  camera.cpp
//  Thanda
//

#include "camera.hpp"
#include "../la.hpp"

using namespace glm;

Camera::Camera() {
    eye = glm::vec3(0,0,-5);
    ref = glm::vec3(0, 0, 0);
    world_up = glm::vec3(0, 1, 0);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    look = glm::vec3(0, 0, -1);
    this->width = 1024;
    this->height = 768;

    recompute();
}

Camera::Camera(int width, int height) {
    eye = glm::vec3(0,0,-5);
    ref = glm::vec3(0, 0, 0);
    world_up = glm::vec3(0, 1, 0);
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    look = glm::vec3(0, 0, -1);
    this->width = width;
    this->height = height;

    recompute();
}

Camera::~Camera() {
}

void Camera::recompute() {
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);
    aspect = width / (float) height;

    viewproj = glm::perspective(fovy, aspect, near_clip, far_clip) * glm::lookAt(eye, ref, up);
    cameraMat = viewproj * modelMat;
}

void Camera::recomputeEye() {
    eye = glm::vec3(glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0))
            * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0))
            * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom))
            * glm::vec4(0, 0, 0, 1));
    look = glm::normalize(ref - eye);
    recompute();
}

glm::mat4 Camera::getCameraMat() {
    return cameraMat;
}


void Camera::recomputeCameraFromInputs(GLFWwindow *window) {
//    static double lastTime = glfwGetTime();

//    double currentTime = glfwGetTime();
//    float deltaTime = float(currentTime - lastTime);

//    // Get mouse position
//    double mouseX, mouseY;
//    glfwGetCursorPos(window, &mouseX, &mouseY);

//    // Reset mouse position for next frame
//    glfwSetCursorPos(window, width/2.0, height/2.0);

//    // Compute new orientation
//    horizontalAngle += mouseSpeed * float(1024/2 - xpos );
//    verticalAngle   += mouseSpeed * float( 768/2 - ypos );

//    // Direction : Spherical coordinates to Cartesian coordinates conversion
//    glm::vec3 direction(
//        cos(verticalAngle) * sin(horizontalAngle),
//        sin(verticalAngle),
//        cos(verticalAngle) * cos(horizontalAngle)
//    );

    if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
        phi -= 5.0f * DEG2RAD;
    }
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
        phi += 5.0f * DEG2RAD;
    }
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
        theta += 5.0f * DEG2RAD;
    }
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
        theta -= 5.0f * DEG2RAD;
    }

    recomputeEye();
}
