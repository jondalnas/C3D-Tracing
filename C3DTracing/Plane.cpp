#include "pch.h"
#include "Plane.h"

std::pair<Geometry::Hit, bool> Plane::intersects(Ray ray) {
    auto denom = ray.dir.dot(normal*-1);

    if (denom < 1e-4) return {};

    auto p0l0 = pos - ray.pos;
    auto t = p0l0.dot(normal*-1) / denom;

    if (t < 0) return {};

    auto hitPos = ray.dir * t + ray.pos;

    return { Hit{ t, hitPos, normal, mat , this }, true };
}