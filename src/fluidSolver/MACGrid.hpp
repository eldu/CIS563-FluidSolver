#ifndef FS_MACGRID_H
#define FS_MACGRID_H

#include "grid.hpp"
#include "../la.hpp"
#include "math.h"

class MACGrid {
    public:
        MACGrid();
        MACGrid(int width, int height, int depth);
        MACGrid(int width, int height, int depth, int cellsize);
        MACGrid(glm::vec3 resolution, glm::vec3 min, glm::vec3 max);

        // Grids
        Grid gridU;
        Grid gridV;
        Grid gridW;
        Grid gridP;

        float cellWidth;
        int resx, resy, resz;
        glm::vec3 min, max;

        glm::vec3 getLocalPos(glm::vec3 world);
    private:
};

#endif
