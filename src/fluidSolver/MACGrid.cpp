#include "MACGrid.hpp"


MACGrid::MACGrid() : MACGrid(10, 10, 10, 1) {}

MACGrid::MACGrid(int width, int height, int depth) : MACGrid(width, height, depth, 1) {}

MACGrid::MACGrid(int width, int height, int depth, int cellsize) {
    min = glm::vec3();
    max = glm::vec3(width * cellsize, height * cellsize, depth * cellsize);

    resx = width;
    resy = height;
    resz = depth;

    // Default cellsize
    cellWidth = cellsize;

    // Initalize all of the grids
    gridU = Grid(resx + 1, resy, resz);
    gridV = Grid(resx, resy + 1, resz);
    gridW = Grid(resx, resy, resz + 1);
    gridP = Grid(resx, resy, resz);
    gridType = Grid(resx, resy, resz);
}


MACGrid::MACGrid(glm::vec3 resolution, glm::vec3 min, glm::vec3 max) {
    this->min = min;
    this->max = max;

    resx = resolution[0];
    resy = resolution[1];
    resz = resolution[2];

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
    gridU = Grid(resx + 1, resy, resz);
    gridV = Grid(resx, resy + 1, resz);
    gridW = Grid(resx, resy, resz + 1);
    gridP = Grid(resx, resy, resz);
}

glm::vec3 MACGrid::getLocalPos(glm::vec3 world) {
    return (world - min)/cellWidth;
}


void MACGrid::create() {
//    GLuint macgrid_idx[];
//    glm::vec3 macgrid_vert_pos[];
//    glm::vec3 macgrid_vert_col[];
}

void MACGrid::draw() {

}

void MACGrid::destroy() {

}
