#pragma once
#include "Vec3.h"
#include "Ray.h"

#include <utility>

class Geometry {
public:
	Vec3 pos;
	Geometry(Vec3 pos) : pos(pos){}

	struct Hit {
		double distance;
		Vec3 pos;
		Vec3 normal;
	};

	virtual std::pair<Hit, bool> intersects(Ray ray) { return std::pair<Hit, bool> { {}, false }; }
};