#include "Scene.h"
#include <iostream>

Vec3 Scene::calculateColor(const Ray &ray) {
    Geometry::Hit *closest = nullptr;

    Vec3 color(0, 0, 0);

    for (auto &g : scene_) {
        auto h = g->intersects(ray);

        if (h.second && (closest == nullptr || h.first.distance < closest->distance)) {
            closest = &h.first;
        }
    }

    if (closest) {
        color = closest->mat.diffusion;
    }

    return color;
}