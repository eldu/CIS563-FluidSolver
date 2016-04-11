#include "MACGrid.hpp"
#include <iostream>


MACGrid::MACGrid() : MACGrid(10, 10, 10, 1) {}

MACGrid::MACGrid(int width, int height, int depth) : MACGrid(width, height, depth, 1) {}

MACGrid::MACGrid(int width, int height, int depth, float cellsize) {
    min = glm::vec3();
    max = glm::vec3(width * cellsize, height * cellsize, depth * cellsize);

    resx = width;
    resy = height;
    resz = depth;

    // Default cellsize
    cellWidth = cellsize;

    // Initalize all of the grids
    gridU = new Grid(resx + 1, resy, resz);
    gridV = new Grid(resx, resy + 1, resz);
    gridW = new Grid(resx, resy, resz + 1);
    gridP = new Grid(resx, resy, resz);
    gridM = new Grid(resx, resy, resz);
}


MACGrid::MACGrid(glm::vec3 resolution, glm::vec3 min, glm::vec3 max) {
    this->min = min;
    this->max = max;

    resx = (int) resolution[0];
    resy = (int) resolution[1];
    resz = (int) resolution[2];

    glm::vec3 calibrate = max - min;
    float cellx = calibrate[0] / resx;
    float celly = calibrate[1] / resy;
    float cellz = calibrate[2] / resz;

    if (fequal(cellx, celly) && fequal(celly, cellz)) {
        // You gave me great parameters
        cellWidth = cellx;
    } else {
       // You messed up so I need to fix your damned mess
        cellWidth = std::max(std::max(cellx, celly), cellz);
        max = min + cellWidth * resolution;
    }

    // Initalize all of the grids
    gridU = new Grid(resx + 1, resy, resz);
    gridV = new Grid(resx, resy + 1, resz);
    gridW = new Grid(resx, resy, resz + 1);
    gridP = new Grid(resx, resy, resz);
    gridM = new Grid(resx, resy, resz);
}

glm::vec3 MACGrid::getLocalP(glm::vec3 world) {
    return (world - min) / cellWidth + glm::vec3(0.5f, 0.5f, 0.5f);
}

glm::vec3 MACGrid::getLocalU(glm::vec3 world) {
    return (world - min) / cellWidth + glm::vec3(0.f, 0.5f, 0.5f);
}

glm::vec3 MACGrid::getLocalV(glm::vec3 world) {
    return (world - min) / cellWidth + glm::vec3(0.5f, 0.f, 0.5f);
}

glm::vec3 MACGrid::getLocalW(glm::vec3 world) {
    return (world - min) / cellWidth + glm::vec3(0.5f, 0.5f, 0.f);
}


void MACGrid::markEdgeCells() {
    // Mark Edge Cells Once
    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            int idx0 = gridM->convertIdx(i, j, 0);
            int idxZ = gridM->convertIdx(i, j, resz - 1);
            gridM->set(idx0, 2.f);
            gridM->set(idxZ, 2.f);
        }

        for (int k = 0; k < resz; k++) {
            int idx0 = gridM->convertIdx(i, 0, k);
            int idxY = gridM->convertIdx(i, resy - 1, k);
            gridM->set(idx0, 2.f);
            gridM->set(idxY, 2.f);
        }
    }

    for (int j = 0; j < resy; j++) {
        for (int k = 0; k < resz; k++) {
            int idx0 = gridM->convertIdx(0, j, k);
            int idxX = gridM->convertIdx(resx - 1, j, k);
            gridM->set(idx0, 2.f);
            gridM->set(idxX, 2.f);
        }
    }
}

// ENFORCE BOUNDARY CONDITIONS
// There should be no flow into or out of solid cells.
// If u is the velocity in the fluid cell and n is the normal
// to the neighboring stationary solid, u·n=0. So, if a fluid cell
// has a solid neighboring cell, set the velocity component that
// points into a neighboring solid cell to zero
void MACGrid::enforceBoundaryConditions() {
    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            for (int k = 0; k < resz; k++) {
                int idx = gridM->getIdx(i, j, k);
                // If I'm a solid cell, then the velocities at ijk of my cell,
                // actually correspond to the cells going into me.
                if (fequal(gridM->get(idx), 2.f)) {
                    gridU->set(idx, 0.f);
                    gridV->set(idx, 0.f);
                    gridW->set(idx, 0.f);
                }
            }

        }
    }
}

void MACGrid::create() {
    MACGRID_IDX_COUNT = 6 * resx * resy * resz;
    MACGRID_VERT_COUNT = (resx + 1) * (resy + 1) * (resz + 1);

    std::vector<GLuint> MACGRID_idx(MACGRID_IDX_COUNT);
    std::vector<glm::vec3> MACGRID_vert_pos(MACGRID_VERT_COUNT);
    std::vector<glm::vec3> MACGRID_vert_col(MACGRID_VERT_COUNT);

    // Set Vertices
    for (int i = 0; i <=resx; i++) {
        for (int j = 0; j <= resy; j++) {
            for (int k = 0; k <= resz; k++) {
                MACGRID_vert_pos[i + j * resx + k * resx * resy] =
                        glm::vec3(i * cellWidth,
                                  j * cellWidth,
                                  k * cellWidth);
            }
        }
    }

    // Set Colors
    for (int i = 0; i <=resx; i++) {
        for (int j = 0; j <= resy; j++) {
            for (int k = 0; k <= resz; k++) {
                MACGRID_vert_col[i + j * resx + k * resx * resy] = glm::vec3(1.f, 0.f, 0.f);
            }
        }
    }

    // Set Indices
    int count = 0;
    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            for (int k = 0; k < resz; k++) {
                int idx = i + j * (resx + 1) + k * (resx + 1) * (resy + 1);
                MACGRID_idx[count++] = idx;
                MACGRID_idx[count++] = idx + 1;
                MACGRID_idx[count++] = idx;
                MACGRID_idx[count++] = idx + resx + 1;
                MACGRID_idx[count++] = idx;
                MACGRID_idx[count++] = idx + (resx + 1 * resy + 1);
            }
        }
    }

    glGenBuffers(1, &indexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, indexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * MACGRID_IDX_COUNT, &MACGRID_idx, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MACGRID_VERT_COUNT, &MACGRID_vert_pos, GL_STATIC_DRAW);

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MACGRID_VERT_COUNT, &MACGRID_vert_col, GL_STATIC_DRAW);
}

void MACGrid::draw() {
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
    glDrawElements(GL_LINES, MACGRID_IDX_COUNT, GL_UNSIGNED_INT, (void*)0);
}

void MACGrid::destroy() {
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteBuffers(1, &indexbuffer);
}
