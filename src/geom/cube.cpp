#include "cube.hpp"
#include "geom.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

static const int BOX_IDX_COUNT = 24;
static const int BOX_VERT_COUNT = 8;

Cube::Cube() {
    this->min = glm::vec3(-1.0f, -1.0f, -1.0f);
    this->max = glm::vec3(1.0f, 1.0f, 1.0f);
}

Cube::Cube(glm::vec3 &min, glm::vec3 &max){
    this->min = min;
    this->max = max;
}

Cube::Cube(float scaleX, float scaleY, float scaleZ) {
    this->min = glm::vec3(-scaleX/2, -scaleY/2, -scaleZ/2);
    this->max = glm::vec3(scaleX/2, scaleY/2, scaleZ/2);
}

Cube::Cube(float scaleX, float scaleY, float scaleZ, float mX, float mY, float mZ) {
    this->min = glm::vec3(-scaleX/2 + mX, -scaleY/2 + mY, -scaleZ/2 + mZ);
    this->max = glm::vec3( scaleX/2 + mX,  scaleY/2 + mY,  scaleZ/2 + mZ);
}

Cube::~Cube() {

}


void Cube::create() {
    GLuint box_idx[BOX_IDX_COUNT];
    glm::vec3 box_vert_pos[BOX_VERT_COUNT];
    glm::vec3 box_vert_col[BOX_VERT_COUNT];

    // Set Vertices
    box_vert_pos[0] = glm::vec3(min[0], max[1], min[2]);
    box_vert_pos[1] = glm::vec3(max[0], max[1], min[2]);
    box_vert_pos[2] = glm::vec3(max[0], max[1], max[2]);
    box_vert_pos[3] = glm::vec3(min[0], max[1], max[2]);
    box_vert_pos[4] = glm::vec3(min[0], min[1], min[2]);
    box_vert_pos[5] = glm::vec3(max[0], min[1], min[2]);
    box_vert_pos[6] = glm::vec3(max[0], min[1], max[2]);
    box_vert_pos[7] = glm::vec3(min[0], min[1], max[2]);


    // Set Indices
    // Top
    box_idx[0] = 0;
    box_idx[1] = 1;
    box_idx[2] = 1;
    box_idx[3] = 2;
    box_idx[4] = 2;
    box_idx[5] = 3;
    box_idx[6] = 3;
    box_idx[7] = 0;

    // Bottom
    box_idx[8] = 4;
    box_idx[9] = 5;
    box_idx[10] = 5;
    box_idx[11] = 6;
    box_idx[12] = 6;
    box_idx[13] = 7;
    box_idx[14] = 7;
    box_idx[15] = 4;

    // Barrel
    box_idx[16] = 0;
    box_idx[17] = 4;
    box_idx[18] = 1;
    box_idx[19] = 5;
    box_idx[20] = 2;
    box_idx[21] = 6;
    box_idx[22] = 3;
    box_idx[23] = 7;

    // Set Color
    for(int i = 0; i < BOX_VERT_COUNT; i++) {
        box_vert_col[i] = col;
    }

    glGenBuffers(1, &indexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * BOX_IDX_COUNT, box_idx, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * BOX_VERT_COUNT, box_vert_pos, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * BOX_VERT_COUNT, box_vert_col, GL_STATIC_DRAW);
}

void Cube::draw() {
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

    // Draw the triangles
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glDrawElements(GL_LINES, BOX_IDX_COUNT, GL_UNSIGNED_INT, (void*)0);
}

void Cube::destroy() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteBuffers(1, &indexbuffer);
}

bool Cube::isCollision() {
    return false;
}
