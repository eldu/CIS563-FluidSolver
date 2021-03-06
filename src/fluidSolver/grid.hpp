#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "../la.hpp"
#include <vector>
#include "fluidParticle.hpp"

// Everything is implemented such that the grid cells go from 0 to 1.

//template <typename T>
class Grid {
public:
    // Constructors
    Grid();
    Grid(const int width, const int height, const int depth);

    // Variables
    int resx, resy, resz; // Resolution of the grid
    std::vector<float> data;
    std::vector<float> numP; // number of particles affecting, useful for velocity splatting

    // May delete
//    float cellWidth;

    void splatVelocity(glm::vec3 pos, float velocity, int dir);
    float distance(glm::vec3 pos, glm::ivec3 IJK, int dir);

    // Get index equations
    int getIdx(glm::vec3 pos);
    int getIdx(float x, float y, float z);

    glm::ivec3 getIJK(glm::vec3 pos);
    glm::ivec3 getIJK(float x, float y, float z);
    glm::ivec3 getIdxFromIdx(int idx);

    int convertIdx(glm::ivec3 idx);
    int convertIdx(int i, int j, int k);

    bool inBounds(glm::ivec3 idx);
    std::vector<glm::ivec3> getNeighborhood(glm::ivec3 ijk);
    std::vector<int> getFaceNeighbors(int i, int j, int k);

    void averageGrid();
    void clearGrid();
    void clearMarkers();

    float TriLERP(glm::vec3 pos);

    void markWalls();

    // Getter functions
    float operator[](const int idx);
    float operator[](const glm::ivec3 &idx);
    float get(const int idx);
    float get(const int i, const int j, const int k);

    // Set Functions
    void set(int idx, float val);
    void set(int i, int j, int k, float val);
    void set(glm::ivec3 ijk, float val);

    // Add functions
    void add(int idx, float val);
    void add(int i, int j, int k, float val);
    
    // Debugging functions
    void toString();

private:
};

//#endif
