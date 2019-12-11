#pragma once
#include "Geometry.h"

class Plane : public Geometry {
public:
    Vec3 normal;
    Plane(Vec3 pos, Vec3 normal, Material mat) : Geometry(pos, mat), normal(normal) {}
    std::pair<Hit, bool> intersects(Ray &ray) override;
};