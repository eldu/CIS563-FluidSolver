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
    fluid->col = glm::vec3(0.f, 0.f, 1.f);
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
                particles.push_back(p);
            }
        }
    }

    numParticles = particles.size();
}

void FluidSolver::create() {
    for (Particle* p : particles) {
        p->create();
    }

}

void FluidSolver::draw() {
    for (Particle* p : particles) {
        p->draw();
    }
}

void FluidSolver::destroy() {
    for (Particle* p : particles) {
        p->destroy();
    }
}
