#include "flipSolver.hpp"

FLIPSolver::FLIPSolver() {

}

FLIPSolver::FLIPSolver(Cube *container, Cube *fluid) {
    this->container = container;
    this->fluid = fluid;
    this->particleSeparation = 0.3f;

    init();
}

FLIPSolver::FLIPSolver(Cube *container, Cube *fluid, float particleSeparation) {
    // TODO
}

FLIPSolver::~FLIPSolver() {

}


// Initialize the grid and particle positions and velocities.
// Using a helper function can simplify this task.
// Saving gridIndex on the particle can be useful later.
void FLIPSolver::init() {
    fillFluid();
    constructMACGrid();
}

void FLIPSolver::fillFluid() {
    for (float x = fluid->min[0]; x < fluid->max[0]; x += particleSeparation) {
        for (float y = fluid->min[1]; y < fluid->max[1]; y += particleSeparation) {
            for (float z = fluid->min[2]; z < fluid->max[2]; z += particleSeparation) {
                Particle* p = new Particle(glm::vec3(x, y, z));
                particles.push_back(p);
            }
        }
    }

    numParticles = particles.size();
}

void FLIPSolver::update(float deltaTime) {

#define WAIT
#ifdef WAIT
    newGrid.gridM->clearMarkers();

    // MARK FLUID
    for (Particle* p : particles) {
        glm::vec3 local = newGrid.getLocalP(p->pos);
//        std::cout << "(" << local[0] << " " << local[1] << " " << local[2] << ") ";
        newGrid.gridM->set(newGrid.gridM->getIdx(local), 1.f); // MARK FLUID
    }

    // TRANSFER PARTICLE TO GRID
    storeParticleVelocityToGrid(); // Splatting

    // ENFORCE BOUNDARY CONDITIONS
    // There should be no flow into or out of solid cells.
    // If u is the velocity in the fluid cell and n is the normal
    // to the neighboring stationary solid, u·n=0. So, if a fluid cell
    // has a solid neighboring cell, set the velocity component that
    // points into a neighboring solid cell to zero
    newGrid.enforceBoundaryConditions();

    // RESOLVE FORCES ON GRID
    // GRAVITY (This is not right. What have I done. lol...)
    newGrid.addGravity(deltaTime);

    // PRESSURE



    // EXTRAPOLATE VELOCITY
    newGrid.velocityExtrapolation();

    // TRANSFER MACGRID TO PARTICLE
    transferVelocityToParticle();
        // FIND PIC VELOCITY
        // FIND FLIP VELOCITY


    // ADVECT, COLLISION DETECTION AND RESPONCE
    for (Particle* p : particles) {
        glm::vec3 newPos = p->pos + p->vel * deltaTime;
        if (glm::any(glm::lessThan(newPos, container->min)) || glm::any(glm::greaterThan(newPos, container->max))) {
            p->vel = glm::vec3();
        } else {
            p->pos = newPos;
        }
    }


#else
    // SIMPLE
    for (Particle* p : particles) {
        // Gravity
        glm::vec3 g = glm::vec3(0, -GRAVITY, 0);
        p->pos = p->pos + p->vel * deltaTime + 0.5f * g * deltaTime * deltaTime;
    }
#endif

    create();
}

void FLIPSolver::constructMACGrid() {
    glm::vec3 min = container->min;
    glm::vec3 max = container->max;

    // glm::vec3 resolution = max - min;
    // TODO: Not hardcode this.... LOL....
    glm::vec3 resolution = glm::vec3(10, 10, 10);

    MACGrid(resolution, min, max);
}

void FLIPSolver::extrapolateVelocity() {

}

void FLIPSolver::storeParticleVelocityToGrid() {
    newGrid.gridU->clearGrid();
    newGrid.gridV->clearGrid();
    newGrid.gridW->clearGrid();

    for(Particle *p : particles) {
        p->gridCell = newGrid.gridP->getIdx(newGrid.getLocalP(p->pos));

        // Get Local Positions wrt grids
        glm::vec3 lu = newGrid.getLocalU(p->pos);
        glm::vec3 lv = newGrid.getLocalV(p->pos);
        glm::vec3 lw = newGrid.getLocalW(p->pos);

        // Splat
        newGrid.gridU->splatVelocity(lu, p->vel[0], 0);
        newGrid.gridV->splatVelocity(lv, p->vel[1], 1);
        newGrid.gridW->splatVelocity(lw, p->vel[2], 2);


//        glm::vec3 local = newGrid.grid(p->pos);

//        std::vector<glm::vec3> neighborhoodU = newGrid.gridU.getNeighborhood(local);
//        std::vector<glm::vec3> neighborhoodV = newGrid.gridV.getNeighborhood(local);
//        std::vector<glm::vec3> neighborhoodW = newGrid.gridW.getNeighborhood(local);

//        glm::vec3 deltaX = glm::abs(local - neighborhoodU[0]);
//        glm::vec3 deltaY = glm::abs(local - neighborhoodV[0]);
//        glm::vec3 deltaZ = glm::abs(local - neighborhoodW[0]);

//        // TODO Check these indices
//        for (glm::vec3 idx : neighborhoodU) {
//            glm::vec3 kernelX = (idx - glm::vec3(0.5, 0, 0)) / deltaX;
//            glm::vec3 kernelY = (idx - glm::vec3(0, 0.5, 0)) / deltaY;
//            glm::vec3 kernelZ = (idx - glm::vec3(0, 0, 0.5)) / deltaZ;
//            // TODO: CLEAN THIS UP SERIOUSLY
//            // FIX OVERRIDED [] OPERATOR
//            newGrid.gridU.data[newGrid.gridU.getIdx(idx)] += kernelX[0] / weight; // TODO: Change hard coded average
//            newGrid.gridV.data[newGrid.gridV.getIdx(idx)] += kernelY[1] / weight;
//            newGrid.gridW.data[newGrid.gridW.getIdx(idx)] += kernelZ[2] / weight;
//        }
    }

    // Average the velocities
    newGrid.gridU->averageGrid();
    newGrid.gridU->averageGrid();
    newGrid.gridU->averageGrid();
}


// This is a lie. I'm sorry.
void FLIPSolver::transferVelocityToParticle() {
    for (Particle *p : particles) {
        p->vel = interpolateVelocity(p->pos);
    }
}

// I messed up my overloaded functions so it has to be like this for now
// TODO: CLEAN UP
glm::vec3 FLIPSolver::interpolateVelocity(const glm::vec3& pos) {
    glm::vec3 lu = newGrid.getLocalU(pos);
    glm::vec3 lv = newGrid.getLocalV(pos);
    glm::vec3 lw = newGrid.getLocalW(pos);

    float newU = newGrid.gridU->TriLERP(pos);
    float newV = newGrid.gridV->TriLERP(pos);
    float newW = newGrid.gridW->TriLERP(pos);

    return glm::vec3(newU, newV, newW);
}

void FLIPSolver::create() {
    for (Particle* p : particles) {
        p->create();
    }
}

void FLIPSolver::draw() {
    for (Particle* p : particles) {
        p->draw();
    }
}

void FLIPSolver::destroy() {
    for (Particle* p : particles) {
        p->destroy();
    }
}


