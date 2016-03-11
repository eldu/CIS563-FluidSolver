#pragma once
#include "geom.hpp"
#include "GL/glew.h"
#include "glm/glm.hpp"

class Cube : public Geometry {
private:
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint indexbuffer;
public:
    Cube();
    Cube(glm::vec3 &min, glm::vec3 &max);
    Cube(float scaleX, float scaleY, float scaleZ);
    ~Cube();

    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 col = glm::vec3(1.0f, 1.0f, 1.0f);

    float minX, minY, minZ, maxX, maxY, maxZ;

    void create();
    void destroy();

    void draw();

    bool isCollision();
};
