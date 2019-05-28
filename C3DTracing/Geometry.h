#pragma once
#include "Vec3.h"
#include "Ray.h"

class Geometry {
public:
	Vec3 pos;
	Geometry(Vec3 pos) : pos(pos){}

	virtual bool intersects(Ray ray) { return false; }
};

