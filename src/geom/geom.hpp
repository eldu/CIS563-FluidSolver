//
//  geom.hpp
//  Thanda

#ifndef FS_GEOM_H
#define FS_GEOM_H

class Geometry {
public:
    Geometry();
    ~Geometry();

    virtual bool isCollision();
    void create();
    void draw();
    void destroy();
};

#endif
