#ifndef FS_TRIANGLE_H
#define FS_TRIANGLE_H

#include "geom.hpp"
#include "GL/glew.h"
#include "glm/glm.hpp"

class Triangle : public Geometry {
public:
    Triangle();
    ~Triangle();

    GLuint vertexbuffer;

    void create();
    void destroy();

    void draw();

    // Inherit from the Geometry Class
    bool isCollision();
};

#endif
