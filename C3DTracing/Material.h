#pragma once

#include "Vec3.h"

class Material {
public:
    Material(Vec3 diffusion, Vec3 emission) : diffusion(diffusion), emission(emission) {}
    Material() : diffusion(), emission() {}

    static Material materialWithDiffusion(Vec3 diffusion) {
        return {diffusion, Vec3(0, 0, 0)};
    }

    static Material materialWithEmision(Vec3 emission) {
        return {Vec3(0, 0, 0), emission};
    }

    Vec3 diffusion;
    Vec3 emission;
};