#pragma once

#include "Vec3.h"

class Math {
public:
    static Vec3 reflect(Vec3 dir, Vec3 normal) {
        return dir - normal * 2 * dir.dot(normal);
    }

    static Vec3 refract(Vec3 dir, Vec3 normal, double n) {
        double c = -(normal).dot(dir);
        return dir * n + normal * (n * c - sqrt(1 - n * n * (1 - c * c)));
    }
};
