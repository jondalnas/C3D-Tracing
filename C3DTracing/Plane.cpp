#include "pch.h"
#include "Plane.h"

std::pair<Geometry::Hit, bool> Plane::intersects(Ray &ray) {
	auto negNorm = -normal;

    auto denom = ray.dir.dot(negNorm);

    if (denom < 0) return {};

    auto p0l0 = pos - ray.pos;
    auto t = p0l0.dot(negNorm) / denom;

    if (t < 0) return {};

    auto hitPos = ray.dir * t + ray.pos;

    return { Hit{ t, hitPos, normal, mat , this }, true };
}