#pragma once
#include "Vec3.h"
#include "Geometry.h"
#include <memory>

class Geometry;

class Ray {
public:
    Ray(Vec3 pos, Vec3 dir) : pos(pos), dir(dir), iterations(0), refractiveIndex(1) {}

	Vec3 pos;
	Vec3 dir;

    unsigned int iterations;
    double refractiveIndex;

    std::shared_ptr<Geometry> inside;
};