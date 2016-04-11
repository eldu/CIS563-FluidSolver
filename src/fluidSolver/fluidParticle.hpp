#ifndef FS_FLUIDPARTICLE_H
#define FS_FLUIDPARTICLE_H


#include "GL/glew.h"
#include "glm/glm.hpp"
#include "../la.hpp"

class Particle {
public:
    Particle();
    Particle(glm::vec3 pos);
    Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 col);
    ~Particle();

    // Parameters
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 col = glm::vec3(0, 0, 1);

    int gridCell;

    // Draw a particle on the screen
    void create();
    void draw();
    void destroy();
private:
    // Buffers
    GLuint vertexbuffer;
    GLuint colorbuffer;
    GLuint indexbuffer;
};

#endif
