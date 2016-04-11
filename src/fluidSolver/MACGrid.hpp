#ifndef FS_MACGRID_H
#define FS_MACGRID_H

#include "grid.hpp"
#include "../la.hpp"
#include "math.h"
#include "../geom/geom.hpp"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "tbb/tbb.h"


class MACGrid : Geometry {
    private:
        GLuint vertexbuffer;
        GLuint colorbuffer;
        GLuint indexbuffer;
    public:
        MACGrid();
        MACGrid(int width, int height, int depth);
        MACGrid(int width, int height, int depth, float cellsize);
        MACGrid(glm::vec3 resolution, glm::vec3 min, glm::vec3 max);

        // Grids
        Grid* gridU;
        Grid* gridV;
        Grid* gridW;
        Grid* gridP;
        Grid* gridM; // Marker Grid

        float cellWidth;
        int resx, resy, resz;
        glm::vec3 min, max;

        // Markers
        void markEdgeCells();
        void enforceBoundaryConditions();

        // Get Local Positions
        glm::vec3 getLocalP(glm::vec3 world);
        glm::vec3 getLocalU(glm::vec3 world);
        glm::vec3 getLocalV(glm::vec3 world);
        glm::vec3 getLocalW(glm::vec3 world);


        // Geometry Drawable Functions
        int MACGRID_IDX_COUNT;
        int MACGRID_VERT_COUNT;
        void create();
        void draw();
        void destroy();
    private:
};

#endif
