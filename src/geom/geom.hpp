//
//  geom.hpp
//  Thanda

#pragma once


class Geometry {
public:
    Geometry();
    ~Geometry();

    virtual bool isCollision();
    void create();
    void draw();
    void destroy();
};
