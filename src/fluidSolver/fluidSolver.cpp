//
//  fluidSolver.cpp
//  Thanda


#include "fluidSolver.hpp"
#include <stdio.h>
#include <string>
#include <fstream> // Stream class to read/write on files
#include <iostream>
#include <sstream>

using namespace std;

FluidSolver::FluidSolver() {

}

FluidSolver::FluidSolver(Cube* container, Cube* fluid, float particleSeparation) {
    this->fluid = fluid;
    this->container = container;
    this->particleSeparation = particleSeparation;

    fillFluid();
}

FluidSolver::~FluidSolver() {

}

void FluidSolver::fillFluid() {
    for (float x = fluid->min[0]; x < fluid->max[0]; x += particleSeparation) {
        for (float y = fluid->min[1]; y < fluid->max[1]; y += particleSeparation) {
            for (float z = fluid->min[2]; z < fluid->max[2]; z += particleSeparation) {
                // Create fluid here
                Particle* p = new Particle(glm::vec3(x, y, z));
//                std::cout << "{" << p->pos[0] <<", " << p->pos[1] << ", " << p->pos[2] << "}" << std::endl;
                pList.push_back(p);
            }
        }
    }
}
