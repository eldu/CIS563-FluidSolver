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
        int idx = convertIdx(cell);

        data[idx] += velocity * dist;
        numP[idx]++; // Update Number of Particles
    }
}

void Grid::averageGrid() {
    for (int i = 0; i < data.size(); i++) {
        if (numP[i] > 1) {
            data[i] /= numP[i];
        }
    }
}

void Grid::clearGrid() {
    for (int i = 0; i < data.size(); i++) {
            numP[i] = 0;
            data[i] = 0;
    }
}

void Grid::clearMarkers() {
    for (int i = 1; i < resx - 1; i++) {
        for (int j = 1; j < resy - 1; j++) {
            for (int k = 1; k < resz - 1; k++) {
                data[convertIdx(i, j, k)] = 0.f;
            }
        }
    }
}

float Grid::TriLERP(glm::vec3 pos) {
    glm::ivec3 ijk = getIJK(pos);
    glm::vec3 dist = glm::abs(pos - glm::vec3(ijk));

    // INTERPOLATE ON X
    // 0, 1
    float cU00 = data[convertIdx(ijk)] * (1.0 - dist[0])
                 + data[convertIdx(ijk[0] + 1, ijk[1], ijk[2])] * dist[0];
    // 7, 2
    float cU01 = data[convertIdx(ijk[0], ijk[1], ijk[2] + 1)] * (1.0 - dist[0])
                 + data[convertIdx(ijk[0] + 1, ijk[1] + 1, ijk[2])] * dist[0];
    // 5, 3
    float cU10 = data[convertIdx(ijk[0], ijk[1] + 1, ijk[2])] * (1.0 - dist[0])
                 + data[convertIdx(ijk[0] + 1, ijk[1], ijk[2] + 1)] * dist[0];
    // 6, 4
    float cU11 = data[convertIdx(ijk[0], ijk[1] + 1, ijk[2] + 1)] * (1.0 - dist[0])
                 + data[convertIdx(ijk[0] + 1, ijk[1] + 1, ijk[2] + 1)] * dist[0];

    // INTERPOLATE ON Y
    // U
    float cU0 = cU00 * (1 - dist[1]) + cU10 * dist[1];
    float cU1 = cU01 * (1 - dist[1]) + cU11 * dist[1];

    // INTERPOLATE ON Z
    // U
    float cU = cU0 * (1 - dist[2]) + cU1 * dist[2];

    return cU;
}

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
std::vector<glm::ivec3> Grid::getNeighborhood(glm::ivec3 ijk) {
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

std::vector<int> Grid::getFaceNeighbors(int i, int j, int k) {
    std::vector<int> result;
    for (int a = -1; a < 2; a += 2) {
        for (int b = 0; b < 3; b++) {
            glm::ivec3 poss = glm::ivec3(i, j, k);
            poss[b] += a;

            int cand = convertIdx(poss);

            if (cand >= 0) {
                result.push_back(cand);
            }
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
float Grid::get(const int i, const int j, const int k) {
    int idx = convertIdx(i, j, k);
    return data[idx];
}

float Grid::get(const int idx) {
    return data[idx];
}

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
        std::cout << "Attempting to set val at index out of bounds";
    } else {
        data[idx] = val;
    }
}

void Grid::add(int idx, float val) {
    if (idx < 0) {
        std::cout << "Attempting to add val at index out of bounds";
    } else {
        data[idx] += val;
    }
}

void Grid::add(int i, int j, int k, float val) {
    int idx = convertIdx(i, j, k);
    add(idx, val);
}
