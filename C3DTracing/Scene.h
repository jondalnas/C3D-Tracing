#pragma once

#include "Vec3.h"
#include "Geometry.h"
#include "Ray.h"
#include <vector>
#include <memory>
#include <random>

class Scene {
    std::vector<std::shared_ptr<Geometry>> scene_;

public:
    Vec3 calculateColor(Ray &ray, std::mt19937 &rng);
    std::vector<std::shared_ptr<Geometry>> inside(Vec3 point);

    void add(std::shared_ptr<Geometry> geometry) {
        scene_.emplace_back(std::move(geometry));
    }
};