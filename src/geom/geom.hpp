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
    virtual void create();
    virtual void draw();
    virtual void destroy();
};

#endif
