#include "grid.hpp"
#include <iostream>

/* * * * * * * * * * * * * * * * * * * * * *
 * Constructors                            *
 * * * * * * * * * * * * * * * * * * * * * */

//template <>
Grid::Grid() {
}

//template <>
Grid::Grid(const int width, const int height, const int depth) {
    resx = width;
    resy = height;
    resz = depth;

    data.resize(resx * resy * resz);
    numP.resize(resx * resy * resz);
}

//template <>
//Grid<bool>::Grid() {
//}

//template <>
//Grid<bool>::Grid(const int width, const int height, const int depth) {
//    resx = width;
//    resy = height;
//    resz = depth;

//    data.resize(resx * resy * resz);
//    numP.resize(resx * resy * resz);
//}

/* * * * * * * * * * * * * * * * * * * * * * * *
 * Local to index                              *
 * * * * * * * * * * * * * * * * * * * * * * * */
// template <typename T>
int Grid::getIdx(float x, float y, float z) {
    int i = (int) x / resx;
    int j = (int) y / resy;
    int k = (int) z / resz;

    // invalid index
//    if (i < 0 || j < 0 || k < 0) {
//        return -1;
//    }

    return i + j * resx + k * resx * resy;
}

// template <typename T>
int Grid::getIdx(glm::vec3 pos) {
    return getIdx(pos[0], pos[1], pos[2]);
}

// template <typename T>
glm::ivec3 Grid::getIJK(glm::vec3 pos) {
    return getIJK(pos[0], pos[1], pos[2]);
}

// template <typename T>
glm::ivec3 Grid::getIJK(float x, float y, float z) {
    int i = (int) x / resx;
    int j = (int) y / resy;
    int k = (int) z / resz;

//    // invalid index
//    if (i < 0 || j < 0 || k < 0) {
//        return -1;
//    }
    return glm::ivec3(i, j, k);
}
/* * * * * * * * * * * * * * * * * * * * * * * *
 * Functions          *
 * * * * * * * * * * * * * * * * * * * * * * * */



// template <typename T>
void Grid::splatVelocity(glm::vec3 pos, float velocity, int dir) {
    glm::ivec3 IJK = getIJK(pos);

    std::vector<glm::ivec3> neighborhood = getNeighborhood(IJK);

    for (glm::ivec3 cell : neighborhood) {
        float dist = fabs(pos[dir] - cell[dir]);
        int idx = getIdx(cell);

        data[idx] += velocity * dist;
        numP[idx]++; // Update Number of Particles
    }
}

void Grid::averageGrid() {
    for (int i = 0; i < data.length; i++) {
        if (numP[i] > 1) {
            data[i] /= numP[i];
        }
    }
}

// pos: Local Position
// idx: Index want to splat
//float Grid::distance(glm::vec3 pos, glm::ivec3 IJK, int dir) {
//    return fabs(pos[direction] - idx[direction]); // Range: [0, 1)
//}


/* * * * * * * * * * * * * * * * * * * * * * * *
 * Index Manipulators  (input index)           *
 * * * * * * * * * * * * * * * * * * * * * * * */
// Relies on valid int idx
// template <typename T>
glm::ivec3 Grid::getIdxFromIdx(int idx) {
    int wait = idx;

    int i = idx % resx;
    int j = (idx / resx) % resy;
    int k = idx / (resx * resy);

    return glm::ivec3(i, j, k);
}

// template <typename T>
int Grid::convertIdx(glm::ivec3 idx) {
    int result = idx[0] + idx[1] * resx + idx[2] * resx * resy;
    return result;
//    if (result >= data.size() || result < 0) {
//        std::cout << "Index is too large";
//        return -1;
//    } else {
//        result;
//    }
}

// template <typename T>
int Grid::convertIdx(int i, int j, int k) {
    return i + j * resx + k * resx * resy;
}


/* * * * * * * * * * * * * * * * * * * * * *
 * Neighborhood                            *
 * * * * * * * * * * * * * * * * * * * * * */
// template <typename T>
std::vector<glm::ivec3> Grid::getNeighborhood(glm::vec3 ijk) {
    std::vector<glm::ivec3> result;
    glm::ivec3 possible[8];

    possible[0] = ijk;
    possible[1] = glm::ivec3(ijk[0] + 1, ijk[1],     ijk[2]);
    possible[2] = glm::ivec3(ijk[0] + 1, ijk[1] + 1, ijk[2]);
    possible[3] = glm::ivec3(ijk[0] + 1, ijk[1],     ijk[2] + 1);
    possible[4] = glm::ivec3(ijk[0] + 1, ijk[1] + 1, ijk[2] + 1);
    possible[5] = glm::ivec3(ijk[0],     ijk[1] + 1, ijk[2]);
    possible[6] = glm::ivec3(ijk[0],     ijk[1] + 1, ijk[2] + 1);
    possible[7] = glm::ivec3(ijk[0],     ijk[1],     ijk[2] + 1);

    for (int i = 0; i < 8; i++) {
        if(inBounds(possible[i])) {
            result.push_back(possible[i]);
        }
    }

    return result;
}

// template <typename T>
bool Grid::inBounds(glm::ivec3 idx) {
    return idx[0] >= 0 && idx[0] < resx &&
           idx[1] >= 0 && idx[1] < resy &&
           idx[2] >= 0 && idx[2] < resz;
}

/* * * * * * * * * * * * * * * * * * * * * *
 * Setters and Getters                     *
 * * * * * * * * * * * * * * * * * * * * * */
// template <typename T>
float Grid::operator[](const int idx) {
    return data[idx];
}

// template <typename T>
float Grid::operator[](const glm::ivec3 &idx) {
    return data[convertIdx(idx)];
}

// template <typename T>
void Grid::set(int i, int j, int k, float val) {
    int idx = convertIdx(i, j, k);
    set(idx, val);
}

// template <typename T>
void Grid::set(glm::ivec3 ijk, float val) {
    int idx = convertIdx(ijk);
    set(idx, val);
}

// template <typename T>
void Grid::set(int idx, float val) {
    if (idx < 0) {
        std::cout << "Attemptting to set val at index out of bounds";
    } else {
        data[idx] = val;
    }
}
