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
        result.specularDamp += mat.specularDamp;
        result.reflectivity += mat.reflectivity;
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
    double specularDamp = 0;
    double reflectivity = 1;
    bool reflective = false;
	double refractiveIndex = 1;
	bool refractive = false;
	double roughness = 1;
};