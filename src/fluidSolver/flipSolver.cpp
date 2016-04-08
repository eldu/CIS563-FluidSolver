#include "flipSolver.hpp"

FLIPSolver::FLIPSolver() {

}

FLIPSolver::FLIPSolver(Cube *container, Cube *fluid) {
    this->container = container;
    this->fluid = fluid;
    this->particleSeparation = 0.3f;

    init();
    constructMACGrid();
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

//void FLIPSolver::fillFluid() {
//    float w = mGrid.cellWidth / numParticlesPerCell;

////    particles.resize(mGrid.resx * mGrid.resy * mGrid.resz * numParticlesPerCell);

//    for (int i = 0; i < mGrid.resx; i++) {
//        for (int j = 0; j < mGrid.resy; j++) {
//            for (int k = 0; k < mGrid.resz; k++) {
//                float x = i * mGrid.cellWidth + mGrid.min[0];
//                float y = j * mGrid.cellWidth + mGrid.min[1];
//                float z = k * mGrid.cellWidth + mGrid.min[2];


//                Particle* p1 = new Particle(glm::vec3(x + 0.25 * mGrid.cellWidth,
//                                                     y + 0.25 * mGrid.cellWidth,
//                                                     z + 0.25 * mGrid.cellWidth));

//                Particle* p2 = new Particle(glm::vec3(x + 0.25 * mGrid.cellWidth,
//                                                     y + 0.25 * mGrid.cellWidth,
//                                                     z + 0.75 * mGrid.cellWidth));

//                Particle* p3 = new Particle(glm::vec3(x + 0.25 * mGrid.cellWidth,
//                                                     y + 0.75 * mGrid.cellWidth,
//                                                     z + 0.25 * mGrid.cellWidth));

//                Particle* p4 = new Particle(glm::vec3(x + 0.25 * mGrid.cellWidth,
//                                                     y + 0.75 * mGrid.cellWidth,
//                                                     z + 0.75 * mGrid.cellWidth));

////                Particle* p5 = new Particle(glm::vec3(x + 0.75 * mGrid.cellWidth,
////                                                     y + 0.25 * mGrid.cellWidth,
////                                                     z + 0.25 * mGrid.cellWidth));

////                Particle* p6 = new Particle(glm::vec3(x + 0.75 * mGrid.cellWidth,
////                                                     y + 0.25 * mGrid.cellWidth,
////                                                     z + 0.75 * mGrid.cellWidth));

////                Particle* p7 = new Particle(glm::vec3(x + 0.75 * mGrid.cellWidth,
////                                                     y + 0.75 * mGrid.cellWidth,
////                                                     z + 0.25 * mGrid.cellWidth));

////                Particle* p8 = new Particle(glm::vec3(x + 0.75 * mGrid.cellWidth,
////                                                     y + 0.75 * mGrid.cellWidth,
////                                                     z + 0.75 * mGrid.cellWidth));

//                particles.push_back(p1);
//                particles.push_back(p2);
//                particles.push_back(p3);
//                particles.push_back(p4);
////                particles.push_back(p5);
////                particles.push_back(p6);
////                particles.push_back(p7);
////                particles.push_back(p8);

////                for (int n = 0; n < numParticlesPerCell; n++) {
////                    Particle* p = new Particle(glm::vec3(x, y ,z);
////                    p->gridCell = glm::vec3(i, j, k); // Save grid cell
////                    particles.push_back(p);
////                }
//            }
//        }
//    }

//    numParticles = particles.size();
//}

void FLIPSolver::update(float deltaTime) {
    for (Particle* p : particles) {

        // Gravity
        glm::vec3 g = glm::vec3(0, -GRAVITY, 0);
        p->pos = p->pos + p->vel * deltaTime + 0.5f * g * deltaTime * deltaTime;
    }

    create();
}

void FLIPSolver::constructMACGrid() {
    glm::vec3 min = fluid->min;
    glm::vec3 max = fluid->max;

    // glm::vec3 resolution = max - min;
    // TODO: Not hardcode this.... LOL....
    glm::vec3 resolution = glm::vec3(3, 2, 2);

    MACGrid(resolution, min, max);
}

void FLIPSolver::storeParticleVelocityToGrid() {
    for(Particle *p : particles) {
//        glm::vec3 local = mGrid.getLocalPos(p->pos);

//        std::vector<glm::vec3> neighborhoodU = mGrid.gridU.getNeighborhood(local);
//        std::vector<glm::vec3> neighborhoodV = mGrid.gridV.getNeighborhood(local);
//        std::vector<glm::vec3> neighborhoodW = mGrid.gridW.getNeighborhood(local);

//        glm::vec3 deltaX = glm::abs(local - neighborhoodU[0] + glm::vec3(-0.5, 0, 0));
//        glm::vec3 deltaY = glm::abs(local - neighborhoodV[0] + glm::vec3(0, -0.5, 0));
//        glm::vec3 deltaZ = glm::abs(local - neighborhoodW[0] + glm::vec3(0, 0, -0.5));

//        // TODO Check these indices
//        for (glm::vec3 idx : neighborhoodU) {
//            glm::vec3 kernelX = (idx - glm::vec3(0.5, 0, 0)) / deltaX;
//            glm::vec3 kernelY = (idx - glm::vec3(0, 0.5, 0)) / deltaY;
//            glm::vec3 kernelZ = (idx - glm::vec3(0, 0, 0.5)) / deltaZ;
//            // TODO: CLEAN THIS UP SERIOUSLY
//            // FIX OVERRIDED [] OPERATOR
//            mGrid.gridU.data[mGrid.gridU.getIdx(idx)] += kernelX[0] / 4.0; // TODO: Change hard coded average
//            mGrid.gridV.data[mGrid.gridV.getIdx(idx)] += kernelY[1] / 4.0;
//            mGrid.gridW.data[mGrid.gridW.getIdx(idx)] += kernelZ[2] / 4.0;
//        }
    }

    // Average the velocities
}


// I messed up my overloaded functions so it has to be like this for now
// TODO: CLEAN UP
glm::vec3 FLIPSolver::interpolateVelocity(const glm::vec3& pos, MACGrid& m) {
//    glm::vec3 local = m.getLocalPos(pos);

//    std::vector<glm::vec3> neighborhoodU = mGrid.gridU.getNeighborhood(local);
//    std::vector<glm::vec3> neighborhoodV = mGrid.gridV.getNeighborhood(local);
//    std::vector<glm::vec3> neighborhoodW = mGrid.gridW.getNeighborhood(local);

//    //
//    glm::vec3 distU = glm::abs(local - neighborhoodU[0] + glm::vec3(-0.5, 0, 0));
//    glm::vec3 distV = glm::abs(local - neighborhoodV[0] + glm::vec3(0, -0.5, 0));
//    glm::vec3 distW = glm::abs(local - neighborhoodW[0] + glm::vec3(0, 0, -0.5));

//    // INTERPOLATE ON X
//    // U
//    float cU00 = mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[0])] * (1.0 - distU[0]) + mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[1])] * distU[0];
//    float cU01 = mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[7])] * (1.0 - distU[0]) + mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[2])] * distU[0];
//    float cU10 = mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[5])] * (1.0 - distU[0]) + mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[3])] * distU[0];
//    float cU11 = mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[6])] * (1.0 - distU[0]) + mGrid.gridU.data[mGrid.gridU.getIdxFromIdxVec3(neighborhoodU[4])] * distU[0];

//    // INTERPOLATE ON Y
//    // U
//    float cU0 = cU00 * (1 - distU[1]) + cU10 * distU[1];
//    float cU1 = cU01 * (1 - distU[1]) + cU11 * distU[1];

//    // INTERPOLATE ON Z
//    // U
//    float cU = cU0 * (1 - distU[2]) + cU1 * distU[2];


//    // INTERPOLATE ON X
//    // V
//    float cV00 = mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[0])] * (1.0 - distV[0]) + mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[1])] * distV[0];
//    float cV01 = mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[7])] * (1.0 - distV[0]) + mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[2])] * distV[0];
//    float cV10 = mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[5])] * (1.0 - distV[0]) + mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[3])] * distV[0];
//    float cV11 = mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[6])] * (1.0 - distV[0]) + mGrid.gridV.data[mGrid.gridV.getIdxFromIdxVec3(neighborhoodV[4])] * distV[0];

//    // INTERPOLATE ON Y
//    // V
//    float cV0 = cV00 * (1 - distV[1]) + cV10 * distV[1];
//    float cV1 = cV01 * (1 - distV[1]) + cV11 * distV[1];

//    // INTERPOLATE ON Z
//    // V
//    float cV = cV0 * (1 - distV[2]) + cV1 * distV[2];

//    // INTERPOLATE ON X
//    // W
//    float cW00 = mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[0])] * (1.0 - distW[0]) + mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[1])] * distW[0];
//    float cW01 = mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[7])] * (1.0 - distW[0]) + mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[2])] * distW[0];
//    float cW10 = mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[5])] * (1.0 - distW[0]) + mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[3])] * distW[0];
//    float cW11 = mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[6])] * (1.0 - distW[0]) + mGrid.gridW.data[mGrid.gridW.getIdxFromIdxVec3(neighborhoodW[4])] * distW[0];

//    // INTERPOLATE ON Y
//    // W
//    float cW0 = cW00 * (1 - distW[1]) + cW10 * distW[1];
//    float cW1 = cW01 * (1 - distW[1]) + cW11 * distW[1];

//    // INTERPOLATE ON Z
//    // W
//    float cW = cW0 * (1 - distW[2]) + cW1 * distW[2];

//    // Return the trilinerally interpolated (U, V, W);
//    return glm::vec3(cU, cV, cW);
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


