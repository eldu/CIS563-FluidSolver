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
    mGrid.gridM->clearMarkers();

    // MARK FLUID
    for (Particle* p : particles) {
        glm::vec3 local = mGrid.getLocalP(p->pos);
        std::cout << "(" << local[0] << " " << local[1] << " " << local[2] << ") ";
//        mGrid.gridM->set(mGrid.gridM->getIdx(local), 1.f); // MARK FLUID
    }

    // TRANSFER PARTICLE TO GRID
    storeParticleVelocityToGrid(); // Splatting

    // ENFORCE BOUNDARY CONDITIONS
    // There should be no flow into or out of solid cells.
    // If u is the velocity in the fluid cell and n is the normal
    // to the neighboring stationary solid, u·n=0. So, if a fluid cell
    // has a solid neighboring cell, set the velocity component that
    // points into a neighboring solid cell to zero
//    mGrid.enforceBoundaryConditions();

    // RESOLVE FORCES ON GRID
    // GRAVITY
    mGrid.addGravity(deltaTime);
    // PRESSURE

    // EXTRAPOLATE VELOCITY



    // TRANSFER MACGRID TO PARTICLE
    transferVelocityToParticle();
        // FIND PIC VELOCITY
        // FIND FLIP VELOCITY
    // ADVECT
    for (Particle* p : particles) {
        p->pos = p->pos + p->vel * deltaTime;
    }
    // COLLISISON DETECTION AND RESPONCE
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
    glm::vec3 resolution = glm::vec3(5, 5, 5);

    MACGrid(resolution, min, max);
}

void FLIPSolver::extrapolateVelocity() {

}

void FLIPSolver::storeParticleVelocityToGrid() {
    mGrid.gridU->clearGrid();
    mGrid.gridV->clearGrid();
    mGrid.gridW->clearGrid();

    for(Particle *p : particles) {
        p->gridCell = mGrid.gridP->getIdx(mGrid.getLocalP(p->pos));

        // Get Local Positions wrt grids
        glm::vec3 lu = mGrid.getLocalU(p->pos);
        glm::vec3 lv = mGrid.getLocalV(p->pos);
        glm::vec3 lw = mGrid.getLocalW(p->pos);

        // Splat
        mGrid.gridU->splatVelocity(lu, p->vel[0], 0);
        mGrid.gridV->splatVelocity(lv, p->vel[1], 1);
        mGrid.gridW->splatVelocity(lw, p->vel[2], 2);


//        glm::vec3 local = mGrid.grid(p->pos);

//        std::vector<glm::vec3> neighborhoodU = mGrid.gridU.getNeighborhood(local);
//        std::vector<glm::vec3> neighborhoodV = mGrid.gridV.getNeighborhood(local);
//        std::vector<glm::vec3> neighborhoodW = mGrid.gridW.getNeighborhood(local);

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
//            mGrid.gridU.data[mGrid.gridU.getIdx(idx)] += kernelX[0] / weight; // TODO: Change hard coded average
//            mGrid.gridV.data[mGrid.gridV.getIdx(idx)] += kernelY[1] / weight;
//            mGrid.gridW.data[mGrid.gridW.getIdx(idx)] += kernelZ[2] / weight;
//        }
    }

    // Average the velocities
    mGrid.gridU->averageGrid();
    mGrid.gridU->averageGrid();
    mGrid.gridU->averageGrid();
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
    glm::vec3 lu = mGrid.getLocalU(pos);
    glm::vec3 lv = mGrid.getLocalV(pos);
    glm::vec3 lw = mGrid.getLocalW(pos);

    float newU = mGrid.gridU->TriLERP(pos);
    float newV = mGrid.gridV->TriLERP(pos);
    float newW = mGrid.gridW->TriLERP(pos);

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


