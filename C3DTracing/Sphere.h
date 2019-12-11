#pragma once
#include "Geometry.h"

class Sphere: public Geometry {
public:
	double radius;
	Sphere(Vec3 pos, double radius, Material mat) : Geometry(pos, mat), radius(radius) {}
	std::pair<Hit, bool> intersects(Ray &ray) override;
	bool inside(Vec3 point) override;
};