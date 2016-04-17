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
    gridA = new Grid(resx, resy, resz);
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


// Setting up matrix entries for the pressure equations
void MACGrid::pressureSolve(float dt, float dx) {
   int n = gridM->data.size();
   Eigen::VectorXd x(n), b(n);
   Eigen::SparseMatrix<float> A(n,n);

   // Set everything to zero otherwise we'll get weird NaNs
   A.setZero();
   x.setZero;
   b.setZero;

   // fill A and b
   float rho = 1.0f;
   float dx = 1.0f; // TODO: Actually cell size?
   float Ascale = dt / (rho * dx * dx);
   float Bscale = 1.0f / dx;

   // Fill A
   typedef Triplet<float> T;
   std::vector<T> tripleList;

   for (int i = 0; i < resx; i++) {
       for (int j = 0; j < resy; j++) {
           for (int k = 0; k < resz; k++) {
               int idx = gridM->convertIdx(i, j, k);
               int idxplusi = gridM->convertIdx(i + 1, j, k);
               int idxplusj = gridM->convertIdx(i, j + 1, k);
               int idxplusk = gridM->convertIdx(i, j, k + 1);
               int idxminusi = gridM->convertIdx(i - 1, j, k);

               int type = (int) gridM->get(idx);
               int typeplusi = (int) gridM->get(idxplusi);
               int typeplusj = (int) gridM->get(idxplusj);
               int typeplusk = (int) gridM->get(idxplusk);
               int typeminusi = (int) gridM->get(idxminusi);

               // A
               if (type == FLUID) {
                   if (typeplusi == FLUID) {
                       tripleList.push_back(Eigen::Triple<float>(idx, idx, Ascale));
                       tripleList.push_back(Eigen::Triple<float>(idxplusi, idx, Ascale));
                       tripleList.push_back(Eigen::Triple<float>(idx, idxplusi, -Ascale)); // TODO: CHECK
                   } else if (typeplusi == EMPTY) {
                       tripleList.push_back(Eigen::Triple<float>(idx, idx, Ascale));
                   }

                   if (typeplusj == FLUID) {
                       tripleList.push_back(Eigen::Triple<float>(idx, idx, Ascale));
                       tripleList.push_back(Eigen::Triple<float>(idxplusj, idx, Ascale));
                       tripleList.push_back(Eigen::Triple<float>(idx, idxplusj, -Ascale)); // TODO: CHECK
                   } else if (typeplusi == EMPTY) {
                       tripleList.push_back(Eigen::Triple<float>(idx, idx, Ascale));
                   }

                   if (typeplusk == FLUID) {
                       tripleList.push_back(Eigen::Triple<float>(idx, idx, Ascale));
                       tripleList.push_back(Eigen::Triple<float>(idxplusk, idx, Ascale));
                       tripleList.push_back(Eigen::Triple<float>(idx, idxplusk, -Ascale)); // TODO: CHECK
                   } else if (typeplusi == EMPTY) {
                       tripleList.push_back(Eigen::Triple<float>(idx, idx, Ascale));
                   }
               }

               // B
               if (typeminusi == solid) {
//                   b[idx] -= Bscale * grid
               }
           }
       }
   }

   A.setFromTriplets(tripletList.begin(), tripletList.end());


   Eigen::ConjugateGradient<SparseMatrix<double> > cg;
   cg.compute(A);
   x = cg.solve(b);
   std::cout << "#iterations:     " << cg.iterations() << std::endl;
   std::cout << "estimated error: " << cg.error()      << std::endl;
   // update b, and solve again
   x = cg.solve(b);

//    float rho = 1.0f;
    float scale = dt / (dx * dx);


    Grid Adiag, Aplusi, Aplusj, Aplusk;

    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            for (int k = 0; k < resz; k++) {
                int m_ijk = (int) gridM->get(i, j, k);

                if (m_ijk == 1) {
                    int other = (int) gridM->get(i+1, j, k);
                    if (other == 1) { // Fluid
                        Adiag.add(m_ijk, scale);
                        Adiag.add(i+1, j, k, scale);
                        Aplusi.set(i, j , k, -scale);
                    } else if (other == 0) { // Empty
                        Adiag.add(i, j, k, scale);
                    }

                    other = (int) gridM->get(i, j+1, k);
                    if (other == 1) { // Fluid
                        Adiag.add(m_ijk, scale);
                        Adiag.add(i, j+1, k, scale);
                        Aplusi.set(i, j , k, -scale);
                    } else if (other == 0) { // Empty
                        Adiag.add(m_ijk, scale);
                    }

                    other = (int) gridM->get(i+1, j, k);
                    if (other == 1) { // Fluid
                        Adiag.add(i, j, k, scale);
                        Adiag.add(i, j, k+1, scale);
                        Aplusi.set(i, j , k, -scale);
                    } else if (other == 0) { // Empty
                        Adiag.add(m_ijk, scale);
                    }
                }

            }
        }
    }

}



void MACGrid::velocityExtrapolation() {
    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            for (int k = 0; k < resz; k++) {
                int idx = gridM->convertIdx(i, j, k);

                if (fequal(gridM->get(idx), 0.f)) {
                    // Face Neighbors
                    for (int nidx : gridM->getFaceNeighbors(i, j, k)) {
                        if (gridM->get(nidx) == 1) { // If neighbor is fluid, mark fluid
                               gridM->set(idx, 1.f);
                               break;
                        }
                    }
                    // Everything is default 0.f = empty or near solid
                    // 2.f = solid (the boundaries)
                }
            }
        }
    }

    for (int i = 0; i < resx; i++) {
        for (int j = 0; j < resy; j++) {
            for (int k = 0; k < resz; k++) {
                int idx = gridM->convertIdx(i, j, k);

                if (fequal(gridM->get(idx), 0.f)) {
                    float numFluid = 0.f;
                    float U = 0.f;
                    float V = 0.f;
                    float W = 0.f;

                    // Face Neighbors
                    for (int nidx : gridM->getFaceNeighbors(i, j, k)) {
                        if (gridM->get(nidx) == 1) {
                            // Neighbor is fluid
                            // TODO: Probably will end up not working.
                            numFluid++;
                            U += gridU->get(nidx);
                            V += gridV->get(nidx);
                            W += gridW->get(nidx);
                        }
                    }

                    // If you have fluid neighbors, get the average of their velocities
                    if (numFluid > 0) {
                        gridU->set(idx, U / numFluid);
                        gridV->set(idx, V / numFluid);
                        gridW->set(idx, W / numFluid);
                    }
                }
            }
        }
    }
}

void MACGrid::addGravity(float deltaTime) {
    for (int n = 0; n < resx * resy * resz; n++) {
//        if (!fqual(gridM->get(idx), 2.f)) {

//        }
        gridV->add(n, 0.5 * -GRAVITY * deltaTime);
    }
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
