//
//  camera.hpp
//  Thanda
//

#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "../la.hpp"

class Camera {
private:
    glm::mat4 cameraMat;
    glm::vec3 eye, ref, up;
public:
    Camera(int width, int height);
    ~Camera();

    glm::mat4 getCameraMat();

    void recomputeEye();

    int width, height;
    float theta = -45 * M_PI / 180.0;
    float phi = -45 * M_PI / 180.0;
    float zoom = 10;
    float fovy = 45 * M_PI / 180.0;
    float near_clip = 0.1f;
    float far_clip = 100.0f;
    float aspect = 4.0f / 3.0f;
};
