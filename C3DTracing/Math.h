#pragma once

#include "Vec3.h"

class Math {
public:
    static Vec3 reflect(Vec3 dir, Vec3 normal) {
        return dir - normal * 2 * dir.dot(normal);
    }

    static Vec3 refract(Vec3 dir, Vec3 normal, double refractiveIndexOfRay, double refractiveIndexOfObject) {
        double cosDir = dir.dot(normal);

        double refractiveIndex = refractiveIndexOfRay / refractiveIndexOfObject;

        if (cosDir < 0) cosDir = -cosDir;
        else normal = normal * -1;

        double k = 1 - refractiveIndex * refractiveIndex * (1 - cosDir * cosDir);
        if (k < 0) return Vec3();
        else return dir * refractiveIndex + normal * (refractiveIndex * cosDir - sqrt(k));
    }

    static double fresnel(Vec3 dir, Vec3 normal, double refractiveIndexOfRay, double refractiveIndexOfObject) {
        double cosDir = dir.dot(normal);

        double sinTarget = refractiveIndexOfRay / refractiveIndexOfObject * sqrt(1 - cosDir * cosDir);

        if (sinTarget >= 1) {
            return 1;
        } else {
            double cosTarget = sqrt(1 - sinTarget * sinTarget);
            cosDir = fabs(cosDir);
            double rs = ((refractiveIndexOfObject * cosDir) - (refractiveIndexOfRay * cosTarget)) / ((refractiveIndexOfObject * cosDir) + (refractiveIndexOfRay * cosTarget));
            double rp = ((refractiveIndexOfRay * cosDir) - (refractiveIndexOfObject * cosTarget)) / ((refractiveIndexOfRay * cosDir) + (refractiveIndexOfObject * cosTarget));

            return (rs * rs + rp * rp) / 2;
        }
    }
};
