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
    Vec3 calculateColor(Ray &ray, std::mt19937 &rng, std::pair<Geometry::Hit, bool> hit);
    std::vector<std::shared_ptr<Geometry>> inside(Vec3 point);

    void add(std::shared_ptr<Geometry> geometry) {
        scene_.emplace_back(std::move(geometry));
    }

    std::pair<Geometry::Hit, bool> findGeometry(Ray &ray) {
        std::unique_ptr<Geometry::Hit> closest = nullptr;

        for (auto &g : scene_) {
            auto h = g->intersects(ray);

            if (h.second && (closest == nullptr || h.first.distance < closest->distance)) {
                Geometry::Hit clone = Geometry::Hit(h.first);
                closest = std::make_unique<Geometry::Hit>(clone);
            }
        }

        if (closest == nullptr) return {};
        else return {closest.get(), true};
    }
};