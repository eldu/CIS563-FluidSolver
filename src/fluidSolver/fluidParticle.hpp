#pragma once;
#include "GL/glew.h"
#include "glm/glm.hpp"

class Particle {
public:
    Particle();
    Particle(glm::vec3 pos);
    Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 col);
    ~Particle();

    // Parameters
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 col;

    // Draw a particle on the screen
    void create();
    void draw();
    void destroy();
private:
    // Buffers
    GLuint vertexbuffer;
    GLuint colorbuffer;
};
