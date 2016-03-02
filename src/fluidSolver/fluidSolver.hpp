//
//  fluidSolver.hpp
//  Thanda

#pragma once
#include "../geom/cube.hpp"

class FluidSolver {
public:
    FluidSolver();
    FluidSolver(Cube* container, Cube* particles, float particleSeparation);

    ~FluidSolver();

};
