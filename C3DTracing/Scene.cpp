#include "pch.h"
#include "Scene.h"
#include "Math.h"
#include <iostream>

Vec3 Scene::calculateColor(Ray &ray, std::mt19937 &rng, std::pair<Geometry::Hit, bool> hit = {}) {
    if (ray.iterations > 3) return Vec3();

    std::unique_ptr<Geometry::Hit> closest = nullptr;

    if (!hit.second) {
        auto h = findGeometry(ray);

        if (h.second) {
            closest = std::make_unique<Geometry::Hit>(h.first);
        }
    } else closest = std::make_unique<Geometry::Hit>(hit.first);

    if (closest) {
        if (!closest->mat.emission.isZero()) return closest->mat.emission;

        Vec3 normal(closest->normal);
        Vec3 pos(closest->pos);
        if (!closest->mat.reflective && !closest->mat.refractive) {
            Vec3 dir = Math::hemisphere(normal, rng);

            double cosTheta = dir.dot(normal);

            //Vec3 reflectDir = Math::reflect(ray.dir, normal);
            //auto BFDR = (powf(std::max(reflectDir.dot(dir), 0.0), closest->mat.specularDamp) * closest->mat.reflectivity / M_PI);

            double pdf = cosTheta * M_1_PI;

            std::uniform_real_distribution<> unit(0.0, 1.0);
            //Russian roulette, we make a value named q and set it to the probability that the ray is getting terminated
            //then we multiply the result with 1/(1-q), so we don't lose any energy from this action
            double q = cosTheta;
            if (unit(rng) > q) return Vec3();//closest->mat.emission + (closest->mat.diffusion * BFDR * dir.dot(normal) * (1 / pdf));

            double loss = 1.0 / q;

            Ray newRay(pos, dir);
            newRay.refractiveIndex = ray.refractiveIndex;
            newRay.iterations = ray.iterations + 1;
            Vec3 newColor = calculateColor(newRay, rng);

            return newColor * closest->mat.diffusion * Math::BRDF(ray.dir*-1, dir, normal, &closest->mat, ray.refractiveIndex) * cosTheta * M_1_PI * loss * (1 / pdf);
        } else {
            Vec3 reflectColor;
            if (closest->mat.reflective) {
                Vec3 reflectDir = Math::reflect(ray.dir, normal);

                Ray newRay(pos, reflectDir);
                newRay.refractiveIndex = ray.refractiveIndex;
                newRay.iterations = ray.iterations + 1;
                reflectColor = calculateColor(newRay, rng);
            }

            if (closest->mat.refractiveIndex != -1) {
                std::shared_ptr<Geometry> insideGeometry;

                for (auto g : inside(pos)) {
                    if (g != ray.inside) {
                        insideGeometry = g;
                        break;
                    }
                }

                double refractiveIndex = insideGeometry == nullptr ? 1 : insideGeometry->mat.refractiveIndex;

                Vec3 refractDir = Math::refract(ray.dir, normal, ray.refractiveIndex, refractiveIndex);

                //If total refraction occurs, then ether do 100% reflection or return diffuse color
                if (refractDir.isZero()) {
                    if (closest->mat.reflective)
                        return reflectColor;
                    return closest->mat.diffusion;
                }

                Ray newRay(pos, refractDir);
                newRay.refractiveIndex = refractiveIndex;
                newRay.iterations = ray.iterations;
                newRay.inside = insideGeometry;
                Vec3 refractColor = calculateColor(newRay, rng);

                if (closest->mat.reflective) {
                    double fr = Math::fresnel(ray.dir, normal, ray.refractiveIndex, refractiveIndex);

                    return reflectColor * fr + refractColor * (1 - fr); //TODO: (reflectColor - refractColor) * fr + refractColor?
                } else {
                    return refractColor;
                }
            }

            return reflectColor;
        }
    }

    return Vec3();
}

std::vector<std::shared_ptr<Geometry>> Scene::inside(Vec3 point) {
    std::vector<std::shared_ptr<Geometry>> result;

    for (auto &g : scene_) {
        if (g->inside(point)) {
            result.emplace_back(g);
        }
    }

    return result;
}