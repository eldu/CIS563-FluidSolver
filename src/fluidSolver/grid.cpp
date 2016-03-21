#include "grid.hpp"

Grid::Grid() {
}

Grid::Grid(int width, int height, int depth) {
    resx = width;
    resy = height;
    resz = depth;
}


int Grid::getIdxFromLocalVec3(glm::vec3 pos) {
    int i = (int) pos[0] / resx;
    int j = (int) pos[1] / resy;
    int k = (int) pos[2] / resz;

    return resx * resx * i + resy * j + k;
}

int Grid::getIdxFromLocal(float x, float y, float z) {
    int i = (int) x / resx;
    int j = (int) y / resy;
    int k = (int) z / resz;

    return resx * resx * i + resy * j + k;
}

glm::vec3 Grid::getIdxFromIdx(int idx) {
    int i = idx / (resx * resx);
    int j = (idx - i * resx * resx) / resy;
    int k = idx - (i * resx * resx + j * resy);
    return glm::vec3(i, j, k);
}

int Grid::getIdxFromIdxVec3(glm::vec3 idx) {
    return idx[0] * resx * resx + idx[1] * resy + idx[2];
}


