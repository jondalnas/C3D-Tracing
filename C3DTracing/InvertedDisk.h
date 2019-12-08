#pragma once

#include "Plane.h"

class InvertedDisk : public Plane {
public:
	double radius;
	InvertedDisk(Vec3 pos, Vec3 normal, float radius, Material mat) : Plane(pos, normal, mat), radius(radius) {}
	std::pair<Hit, bool> intersects(Ray ray) override;
};