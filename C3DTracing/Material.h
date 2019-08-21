#pragma once

#include "Vec3.h"

class Material {
public:
    Material(Vec3 diffusion, Vec3 emission) : diffusion(diffusion), emission(emission) {}
    Material(Vec3 diffusion, double specularDamp, double reflectivity) : diffusion(diffusion), specularDamp(specularDamp), reflectivity(reflectivity) {}
    Material() : diffusion(), emission() {}

    static Material materialWithDiffusion(Vec3 diffusion) {
        return {diffusion, Vec3(0, 0, 0)};
    }

    static Material materialWithEmission(Vec3 emission) {
        return {Vec3(0, 0, 0), emission};
    }

    Vec3 diffusion;
    Vec3 emission;
    double specularDamp = 0;
    double reflectivity = 0;
};