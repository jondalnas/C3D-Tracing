#pragma once
#include "Vec3.h"

class Ray {
public:
	Ray(Vec3 pos, Vec3 dir) : pos(pos), dir(dir), itterations(0) {}

	Vec3 pos;
	Vec3 dir;

    unsigned int itterations;
};