#ifndef FS_GRID_H
#define FS_GRID_H

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "../la.hpp"
#include <vector>

// Everything is implemented such that the grid cells go from 0 to 1.

class Grid {
public:
    // Constructors
    Grid();
    Grid(int width, int height, int depth);

    // Variables
    int resx, resy, resz; // Resolution of the grid
    std::vector<float> data;

    // May delete
    float cellWidth;

    // Functions
    // Get index equations
    int getIdxFromLocalVec3(glm::vec3 pos);
    int getIdxFromLocal(float x, float y, float z);
    glm::vec3 getIdxFromIdx(int idx);
    int getIdxFromIdxVec3(glm::vec3 idx);

    std::vector<glm::vec3> getNeighborhood(glm::vec3 pos);

    // Getter functions
    float operator[](const int idx);
    float operator[](const glm::vec3 &idx);

private:
};

#endif
