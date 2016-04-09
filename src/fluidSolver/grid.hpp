#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "../la.hpp"
#include <vector>

// Everything is implemented such that the grid cells go from 0 to 1.

template <typename T>
class Grid {
public:
    // Constructors
    Grid();
    Grid(const int width, const int height, const int depth);

    // Variables
    int resx, resy, resz; // Resolution of the grid
    std::vector<T> data;

    // May delete
    float cellWidth;

    // Functions


    // Get index equations
    int getIdx(glm::vec3 pos);
    int getIdx(float x, float y, float z);
    glm::vec3 getIdxFromIdx(int idx);

    int convertIdx(glm::vec3 idx);
    int convertIdx(int i, int j, int k);

    std::vector<glm::vec3> getNeighborhood(glm::vec3 pos);

    // Getter functions
    float operator[](const int idx);
    float operator[](const glm::vec3 &idx);

    // Set Functions
    void set(int idx, float val);
    void set(int i, int j, int k, float val);
    void set(glm::vec3 ijk, float val);

//    int main();

private:
};

//#endif
