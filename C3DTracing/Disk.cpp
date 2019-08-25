#include "Disk.h"

std::pair<Geometry::Hit, bool> Disk::intersects(Ray *ray) {
    auto hit = Plane::intersects(ray);

    if (!hit.second) return hit;

    auto v = hit.first.pos - pos;
    auto r = v.lengthSqrt();
    if (r > radius * radius) return {};

    return hit;
}