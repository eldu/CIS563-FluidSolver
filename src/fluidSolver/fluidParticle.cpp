#include "fluidParticle.hpp"

Particle::Particle() {
    this->pos = glm::vec3(-1.0, -1.0, 0.0f);
    this->vel = glm::vec3();
    this->col = glm::vec3(0, 0, 1);
}

Particle::Particle(glm::vec3 pos) {
    this->pos = pos;
    this->vel = glm::vec3(0.0f);
    this->col = glm::vec3(0, 0, 1); // Default blue color
}

Particle::Particle(glm::vec3 pos, glm::vec3 vel, glm::vec3 col) {
    this->pos = pos;
    this->vel = vel;
    this->col = col;
}

Particle::~Particle() {

}

// TODO Refactor this
// This is so unnecesarily complicated
void Particle::create() {
    static const GLfloat g_vertex_buffer_data[] = {
        pos[0], pos[1], pos[2]
    };

    static const GLfloat g_color_buffer_data[] = {
        col[0], col[1], col[2]
    };

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

void Particle::draw() {
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    // Draw the points
    glPointSize(5); // Adjust size of the points displayed
    glDrawArrays(GL_POINTS, 0, 3);

}

void Particle::destroy() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
}
