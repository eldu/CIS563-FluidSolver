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
    glm::vec3 vertex_buffer_data[1];
    glm::vec3 color_buffer_data[1];
    GLuint index_buffer_data[1];

    vertex_buffer_data[0] = pos;
    color_buffer_data[0] = col;
    index_buffer_data[0] = 0;

    glGenBuffers(1, &indexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), index_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), vertex_buffer_data, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint), color_buffer_data, GL_STATIC_DRAW);
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
//    glDrawArrays(GL_POINTS, 0, 1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (void*)0);

}

void Particle::destroy() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
}
