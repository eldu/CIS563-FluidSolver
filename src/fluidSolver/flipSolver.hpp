#ifndef FS_FLIPSOLVER_H
#define FS_FLIPSOLVER_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "../la.hpp"
#include "fluidSolver.hpp"
#include "MACGrid.hpp"
#include "math.h"
#include "tbb/tbb.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace tbb;

class FLIPSolver : FluidSolver {
public:
    FLIPSolver();
    FLIPSolver(Cube* container, Cube* fluid, int rx, int ry, int rz);
    FLIPSolver(Cube* container, Cube* fluid, float particleSeparation);

    ~FLIPSolver();

    Cube* container;
    Cube* fluid;

    MACGrid* oldGrid;
    MACGrid* newGrid;
    
    glm::vec3 resolution;

    // Create the MACGrid
    void constructMACGrid();

    // Initialize the grid and particle positions and velocities.
    // Using a helper function can simplify this task.
    // Saving gridIndex on the particle can be useful later.
    void init();

    // Calculate a weighted average of the particles' velocities in
    // the neighborhood (define a neighborhood of 1 cell width) and
    // store these values on corresponding grid cells. A simple stiff
    // kernel can be used to calculate the weight for the average velocity.
    void storeParticleVelocityToGrid();

    // Calculate the trilinear interpolation for a velocity value at particle position.
    // Using the worldPos of the particle, find the cell index (i,j,k) in the grid.
    // Note that the grids are staggered differently. So, you will need find the actual
    // gridPos to get the correct index (i,j,k). Using this index, we interpolate
    // separately for each component. Think of how you want to design your function
    // calls for good modularity and code reuse
    glm::vec3 interpolateVelocity(MACGrid &g, glm::vec3& pos);

    void extrapolateVelocity();

    void transferVelocityToParticle();

    void fillFluid();
    void update(float deltaTime);

    void create();
    void draw();
    void destroy();

private:
};


#endif
