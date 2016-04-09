#include "grid.hpp"
#include <iostream>

/* * * * * * * * * * * * * * * * * * * * * *
 * Constructors                            *
 * * * * * * * * * * * * * * * * * * * * * */

template <>
Grid<float>::Grid() {
}

template <>
Grid<float>::Grid(const int width, const int height, const int depth) {
    resx = width;
    resy = height;
    resz = depth;

//    data = new data<T>(resx * resy * resz);
    data.resize(resx * resy * resz);
}

template <>
Grid<bool>::Grid() {
}

template <>
Grid<bool>::Grid(const int width, const int height, const int depth) {
    resx = width;
    resy = height;
    resz = depth;

//    data = new data<T>(resx * resy * resz);
    data.resize(resx * resy * resz);
}

/* * * * * * * * * * * * * * * * * * * * * * * *
 * Local to index                              *
 * * * * * * * * * * * * * * * * * * * * * * * */
template <typename T>
int Grid<T>::getIdx(float x, float y, float z) {
    int i = (int) x / resx;
    int j = (int) y / resy;
    int k = (int) z / resz;

    // invalid index
    if (i < 0 || j < 0 || k < 0) {
        return -1;
    }

    return i + j * resx + k * resx * resy;
}

template <typename T>
int Grid<T>::getIdx(glm::vec3 pos) {
    return getIdx(pos[0], pos[1], pos[2]);
}

/* * * * * * * * * * * * * * * * * * * * * * * *
 * Index Manipulators  (input index)           *
 * * * * * * * * * * * * * * * * * * * * * * * */
// Relies on valid int idx
template <typename T>
glm::vec3 Grid<T>::getIdxFromIdx(int idx) {
    int wait = idx;

    int i = idx % resx;
    int j = (idx / resx) % resy;
    int k = idx / (resx * resy);

    return glm::vec3(i, j, k);
}

// TODO: CLEAN THIS UP AND MAKE SURE NO INDEX OUT OF BOUNDS
template <typename T>
int Grid<T>::convertIdx(glm::vec3 idx) {
    int result = idx[0] + idx[1] * resx + idx[2] * resx * resy;
    if (result >= data.size()) {
        std::cout << "Index is too large";
        return -1;
    } else {
        result;
    }
}

template <typename T>
int Grid<T>::convertIdx(int i, int j, int k) {
    return i + j * resx + k * resx * resy;
}


/* * * * * * * * * * * * * * * * * * * * * *
 * Neighborhood                            *
 * * * * * * * * * * * * * * * * * * * * * */
template <typename T>
std::vector<glm::vec3> Grid<T>::getNeighborhood(glm::vec3 pos) {
    int idx = getIdx(pos);
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

/* * * * * * * * * * * * * * * * * * * * * *
 * Setters and Getters                     *
 * * * * * * * * * * * * * * * * * * * * * */
template <typename T>
float Grid<T>::operator[](const int idx) {
    return data[idx];
}

template <typename T>
float Grid<T>::operator[](const glm::vec3 &idx) {
    return data[convertIdx(idx)];
}

template <typename T>
void Grid<T>::set(int i, int j, int k, float val) {
    int idx = convertIdx(i, j, k);
    set(idx, val);
}

template <typename T>
void Grid<T>::set(glm::vec3 ijk, float val) {
    int idx = convertIdx(ijk);
    set(idx, val);
}

template <typename T>
void Grid<T>::set(int idx, float val) {
    if (idx < 0) {
        std::cout << "Attemptting to set val at index out of bounds";
    } else {
        data[idx] = val;
    }
}


//int main() {
////    std::cout << "HELLO";
////    Grid<float> a = new Grid<float>();
//}
