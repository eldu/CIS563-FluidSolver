//
//  camera.hpp
//  Thanda
//

#ifndef FS_CAMERA_H
#define FS_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "../la.hpp"

class Camera {
private:
    glm::mat4 cameraMat, viewproj;
    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::vec3 eye, ref, world_up, up, look, right;
public:
    Camera();
    Camera(int width, int height);
    ~Camera();

    glm::mat4 getCameraMat();

    void recompute();
    void recomputeEye();

    // TODO: Make this better
    void recomputeCameraFromInputs(GLFWwindow* window);
    float speed = 3.0f; // 3 units / second
    float mouseSpeed = 0.005f;

    int width, height;
//    float theta = -45 * M_PI / 180.0;
//    float phi = 45 * M_PI / 180.0;
    float theta = 0.f;
    float phi = 0.f;
    float zoom = 30;
    float fovy = 45 * M_PI / 180.0;
    float near_clip = 0.1f;
    float far_clip = 100.0f;
    float aspect = 4.0f / 3.0f;
};

#endif
