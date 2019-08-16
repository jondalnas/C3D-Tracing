#include "Scene.h"
#include <iostream>

Vec3 Scene::calculateColor(const Ray &ray) {
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
            Vec3 dir(closest->normal);
            Ray newRay(closest->pos, dir);
            newRay.itterations = ray.itterations + 1;
            Vec3 newColor = calculateColor(newRay);

            if (!newColor.isZero()) color = newColor * 0.5 + closest->mat.diffusion * 0.5;
            else color = Vec3(0, 0, 0);
        } else
            color = Vec3(closest->mat.emission);
    }

    return color;
}