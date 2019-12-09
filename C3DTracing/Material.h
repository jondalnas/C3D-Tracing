#pragma once

#include "Vec3.h"

class Material {
public:
    Material(Vec3 diffusion, Vec3 emission) : diffusion(diffusion), emission(emission) {}
    Material(Vec3 diffusion, double refractiveIndex, double roughness) : diffusion(diffusion), refractiveIndex(refractiveIndex), roughness(roughness) {}
    Material() : diffusion(), emission() {}

    static Material materialWithDiffusion(Vec3 diffusion) {
        return {diffusion, Vec3(0, 0, 0)};
    }

    static Material materialWithEmission(Vec3 emission) {
        return {Vec3(0, 0, 0), emission};
    }

    static Material reflectiveMaterial() {
        Material mat = Material();
        mat.reflective = true;
        return mat;
    }

    static Material refractiveMaterial(double refractiveIndex) {
        Material mat = Material();
        mat.refractiveIndex = refractiveIndex;
        return mat;
    }

    Material operator+(Material mat) {
        Material result = *this;

        result.diffusion += mat.diffusion;
        result.emission += mat.emission;
		if (mat.reflective)
			result.reflective = mat.reflective;
		if (mat.refractive)
			result.refractive = mat.refractive;
        if (mat.refractiveIndex != -1 && result.refractiveIndex != -1) result.refractiveIndex += mat.refractiveIndex;
        else if (result.refractiveIndex == -1 && result.refractiveIndex != -1) result.refractiveIndex = mat.refractiveIndex;

        return result;
    }

    Vec3 diffusion;
    Vec3 emission;
    bool reflective = false;
	double refractiveIndex = 0;
	bool refractive = false;
	double roughness = 1;
};