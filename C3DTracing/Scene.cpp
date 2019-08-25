#include "Scene.h"
#include "Math.h"
#include <iostream>

Vec3 Scene::calculateColor(Ray &ray, std::mt19937 &rng) {
    if (ray.itterations > 5) return Vec3(0, 0, 0);

    Geometry::Hit *closest = nullptr;

    for (auto &g : scene_) {
        auto h = g->intersects(&ray);

        if (h.second && (closest == nullptr || h.first.distance < closest->distance)) {
            Geometry::Hit clone = Geometry::Hit(h.first);
            closest = &clone;
        }
    }

    if (closest) {
        std::uniform_real_distribution<> unit(0.0, 1.0);

        Vec3 normal(closest->normal);
        Vec3 pos(closest->pos);
        if (!closest->mat.reflective && closest->mat.refractiveIndex == -1) {
            Vec3 tangent(normal.cross(abs(normal.x) == 1 ? Vec3(0.0, 1.0, 0.0) : Vec3(1.0, 0.0, 0.0)).normalize());
            Vec3 bitangent(normal.cross(tangent));
            Vec3 dir((normal * unit(rng) + tangent * (unit(rng) * 2 - 1) +
                      bitangent * (unit(rng) * 2 - 1)).normalize());

            Ray newRay(pos, dir);
            newRay.refractiveIndex = ray.refractiveIndex;
            newRay.itterations = ray.itterations + 1;
            Vec3 newColor = calculateColor(newRay, rng);

            Vec3 reflectDir = Math::reflect(ray.dir, normal);
            double BFDR =
                    powf(std::max(reflectDir.dot(dir), 0.0), closest->mat.specularDamp) * closest->mat.reflectivity;

            return closest->mat.emission + (newColor * closest->mat.diffusion * (1 + BFDR));
        } else {
            Vec3 reflectColor;
            if (closest->mat.reflective) {
                Vec3 reflectDir = Math::reflect(ray.dir, normal);

                Ray newRay(pos, reflectDir);
                newRay.refractiveIndex = ray.refractiveIndex;
                newRay.itterations = ray.itterations + 1;
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
                newRay.itterations = ray.itterations;
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