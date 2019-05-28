#pragma once
#include "Vec3.h"

class Ray {
public:
	Ray(Vec3 pos, Vec3 dir) : pos(pos), dir(dir) {}

	Vec3 pos;
	Vec3 dir;
};