#include "Scene.h"
#include <iostream>
#include <vector>

Vec3 Scene::calculateColor(const Ray &ray, std::mt19937 &rng) {
    if (ray.itterations > 3) return Vec3(0, 0, 0);

    Geometry::Hit *closest = nullptr;

    Vec3 color(0, 0, 0);

    for (auto &g : scene_) {
        auto h = g->intersects(ray);

        if (h.second && (closest == nullptr || h.first.distance < closest->distance)) {
            Geometry::Hit clone = Geometry::Hit(h.first);
            closest = &clone;
        }
    }

    if (closest) {
        if (closest->mat.emission.isZero()) {
            std::uniform_real_distribution<> unit(0.0, 1.0);

            Vec3 normal(closest->normal);
            Vec3 pos(closest->pos);
            Vec3 tangent(normal.cross(abs(normal.x) == 1 ? Vec3(0.0, 1.0, 0.0) : Vec3(1.0, 0.0, 0.0)));
            Vec3 bitangent(normal.cross(tangent));

            Vec3 dir((normal * unit(rng) + tangent * (unit(rng) * 2 - 1) + bitangent * (unit(rng) * 2 - 1)).normalize());

            Ray newRay(pos, dir);
            newRay.itterations = ray.itterations + 1;
            Vec3 newColor = calculateColor(newRay, rng);

            if (!newColor.isZero()) color = newColor * 0.5 + closest->mat.diffusion * 0.5;
            else color = Vec3(0, 0, 0);
        } else
            color = Vec3(closest->mat.emission);
    }

    return color;
}