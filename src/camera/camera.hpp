//
//  camera.hpp
//  Thanda
//

#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

class Camera {
private:
    glm::mat4 cameraMat;
    glm::vec3 eye, ref, up;
public:
    Camera(int width, int height4);
    ~Camera();

    void create();

    glm::mat4 getCameraMat();
    glm::mat4 getViewProj();

//    void RecomputeEye();
//    void adjustPhi(float p);
//    void adjustTheta(float t);

    int width, height;
    float theta = -45 * M_PI / 180.0;
    float phi = -45 * M_PI / 180.0;
    float zoom = 10;
    float fovy = 45 * M_PI / 180.0;
    float near_clip = 0.1f;
    float far_clip = 1000;
};

//void adjustPhi(Camera c, float p);
//void adjustTheta(Camera c, float t);
