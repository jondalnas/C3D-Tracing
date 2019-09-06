#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"

#include <utility>
#include <memory>

class Geometry {
public:
	Vec3 pos;
	Material mat;
    Geometry(Vec3 pos, Material mat) : pos(pos), mat(mat) {}

    struct Hit {
        Hit() {}
        Hit(double distance, Vec3 pos, Vec3 normal, Material mat, Geometry *hitGeometry) : distance(distance), pos(pos), normal(normal), mat(mat), hitGeometry(hitGeometry) {}
        Hit(Hit *hit) {distance = hit->distance; pos = hit->pos; normal = hit->normal; mat = hit->mat; hitGeometry = hit->hitGeometry;}

        double distance;
        Vec3 pos;
        Vec3 normal;
        Material mat;
        Geometry *hitGeometry;
    };

	virtual std::pair<Hit, bool> intersects(Ray) { return std::pair<Hit, bool> { {}, false }; }
	virtual bool inside(Vec3) { return false; }
};