#pragma once
#include "Geometry.h"

class Sphere: public Geometry {
public:
	double radius;
	Sphere(Vec3 pos, double radius) : Geometry(pos), radius(radius) {}
	std::pair<Hit, bool> intersects(Ray ray) override;
};