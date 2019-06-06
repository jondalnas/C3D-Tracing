#pragma once

#include "Vec3.h"
#include "Geometry.h"
#include "Ray.h"
#include <vector>
#include <memory>

class Scene {
    std::vector<std::unique_ptr<Geometry>> scene_;

public:
    Vec3 calculateColor(const Ray &ray);

    void add(std::unique_ptr<Geometry> geometry) {
        scene_.emplace_back(std::move(geometry));
    }
};