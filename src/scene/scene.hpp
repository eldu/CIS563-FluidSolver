//////
//////  scene.hpp
//////  Thanda
///
#ifndef FS_SCENE_H
#define FS_SCENE_H

#pragma once
#include <vector>
#include "../camera/camera.hpp"
#include "../geom/geom.hpp"

class Scene {
public:
    Scene();

    // All of the objects in the scene
    std::vector<Geometry*> objects = {};
    void create();
    void draw();
    void destroy();

    Camera camera;


    void setCamera(const Camera &c);

};

#endif
