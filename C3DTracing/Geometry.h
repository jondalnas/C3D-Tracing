#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

#include <utility>

class Geometry {
public:
	Vec3 pos;
	Material mat;
	Geometry(Vec3 pos, Material mat) : pos(pos), mat(mat) {}

	struct Hit {
		double distance;
		Vec3 pos;
		Vec3 normal;
		Material mat;
	};

	virtual std::pair<Hit, bool> intersects(Ray) { return std::pair<Hit, bool> { {}, false }; }
};