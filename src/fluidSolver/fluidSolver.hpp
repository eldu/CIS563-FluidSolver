//
//  fluidSolver.hpp
//  Thanda

#pragma once
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
    std::vector<Particle *> pList;

    void fillFluid();
private:

};
