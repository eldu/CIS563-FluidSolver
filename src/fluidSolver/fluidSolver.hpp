//
//  fluidSolver.hpp
//  Thanda
#ifndef FS_FLUIDSOLVER_H
#define FS_FLUIDSOLVER_H

#include "../geom/cube.hpp"
#include <stdio.h>
#include <string>
#include "../la.hpp"
#include "fluidParticle.hpp"
#include <vector>

using namespace std;

class FluidSolver {
public:
    FluidSolver();
    FluidSolver(Cube* container, Cube* fluid, float particleSeparation);

    ~FluidSolver();

    Cube* container;
    Cube* fluid;
    float particleSeparation;

    // List of particles
    std::vector<Particle *> particles;
    int numParticles = 0;

    virtual void fillFluid();
    void update(float deltaTime);

    // Draw Particles on the screen
    void create();
    void draw();
    void destroy();
private:

};


#endif
