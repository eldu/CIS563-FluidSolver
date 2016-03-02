//
//  camera.cpp
//  Thanda
//

#include "camera.hpp"
#include "../la.hpp"
//#define GLM_FORCE_RADIANS

using namespace glm;

Camera::Camera(int width, int height) {
    eye = glm::vec3(4,3,3);
    ref = glm::vec3(0, 0, 0);
    up = glm::vec3(0, 1, 0);
    this->width = width;
    this->height = height;
//    eye = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom)) * glm::vec4(0, 0, 0, 1);
//    ref = glm::vec4(0, 0, 0, 1);
//    up = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::vec4(0, 1, 0, 0);

    glm::mat4 viewMat = glm::lookAt(
                                glm::vec3(eye), // Camera is at (4,3,3), in World Space
                                glm::vec3(ref), // and looks at the origin
                                glm::vec3(up)  // Head is up (set to 0,-1,0 to look upside-down)
                           );

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 projectionMat = glm::perspective(fovy, aspect, near_clip, far_clip);

    glm::mat4 modelMat = glm::mat4(1.0f);

    cameraMat = projectionMat * viewMat * modelMat;
}

Camera::~Camera() {
}

void Camera::recomputeEye() {

}

glm::mat4 Camera::getCameraMat() {
    return cameraMat;
}

//void Camera::RecomputeEye()
//{
//    eye = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, zoom)) * glm::vec4(0, 0, 0, 1);
//    up = glm::rotate(glm::mat4(1.0f), theta, glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), phi, glm::vec3(1, 0, 0)) * glm::vec4(0, 1, 0, 0);
//}


