#pragma once
#include "geom.hpp"
#include "GL/glew.h"
#include "glm/glm.hpp"

class Cube : public Geometry {
private:
    GLuint vertexbuffer;
    GLuint colorbuffer;
public:
    Cube();
    Cube(glm::vec3 &min, glm::vec3 &max);
    Cube(float scaleX, float scaleY, float scaleZ);
    ~Cube();

    glm::vec3 min;
    glm::vec3 max;

    void create();
    void destroy();

    void draw();

    bool isCollision();
};
