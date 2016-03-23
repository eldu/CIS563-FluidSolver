#include "grid.hpp"

Grid::Grid() {
}

Grid::Grid(int width, int height, int depth) {
    resx = width;
    resy = height;
    resz = depth;

    data.resize(resx * resy * resz);
}


int Grid::getIdxFromLocalVec3(glm::vec3 pos) {
    return getIdxFromLocal(pos[0], pos[1], pos[2]);
}

int Grid::getIdxFromLocal(float x, float y, float z) {
    int i = (int) x / resx;
    int j = (int) y / resy;
    int k = (int) z / resz;

    // invalid index
    if (i < 0 || j < 0 || k < 0) {
        return -1;
    }

    return resx * resx * i + resy * j + k;
}

// Relies on valid int idx
glm::vec3 Grid::getIdxFromIdx(int idx) {
    int i = idx / (resx * resx);
    int j = (idx - i * resx * resx) / resy;
    int k = idx - (i * resx * resx + j * resy);

    return glm::vec3(i, j, k);
}

// TODO: CLEAN THIS UP AND MAKE SURE NO INDEX OUT OF BOUNDS
int Grid::getIdxFromIdxVec3(glm::vec3 idx) {
//    if (glm::all(glm::greaterThan(idx, glm::vec3())) &&
//        glm::all(glm::lessThan(idx, glm::vec3(resx, resy, resz)))) {
        return idx[0] * resx * resx + idx[1] * resy + idx[2];
//    } else {
//        return -1;
//    }
}

std::vector<glm::vec3> Grid::getNeighborhood(glm::vec3 pos) {
    int idx = getIdxFromLocalVec3(pos);
    glm::vec3 ijk = getIdxFromIdx(idx);

//    int indexes[8];
    glm::vec3 possible[8];
    std::vector<glm::vec3> result;

    possible[0] = ijk;
    possible[1] = glm::vec3(ijk[0] + 1, ijk[1],     ijk[2]);
    possible[2] = glm::vec3(ijk[0] + 1, ijk[1] + 1, ijk[2]);
    possible[3] = glm::vec3(ijk[0] + 1, ijk[1],     ijk[2] + 1);
    possible[4] = glm::vec3(ijk[0] + 1, ijk[1] + 1, ijk[2] + 1);
    possible[5] = glm::vec3(ijk[0],     ijk[1] + 1, ijk[2]);
    possible[6] = glm::vec3(ijk[0],     ijk[1] + 1, ijk[2] + 1);
    possible[7] = glm::vec3(ijk[0],     ijk[1],     ijk[2] + 1);

    for (int i = 0; i < 8; i++) {
//        indexes[i] = getIdxFromIdxVec3(possible[i]);

//        if (indexes[i] >= 0) {
            result.push_back(possible[i]);
//        }
    }



    return result;
}

float Grid::operator[](const int idx) {
    return data[idx];
}

float Grid::operator[](const glm::vec3 &idx) {
    return data[getIdxFromIdxVec3(idx)];
}
