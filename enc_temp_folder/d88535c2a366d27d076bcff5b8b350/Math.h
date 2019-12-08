#pragma once

#include "Vec3.h"
#include <memory>
#include <random>

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

        double sinTargetSqrt = (refractiveIndexOfRay * refractiveIndexOfRay) / (refractiveIndexOfObject * refractiveIndexOfObject) * (1 - cosDir * cosDir);

        if (sinTargetSqrt >= 1) {
            return 1;
        } else {
            double cosTarget = sqrt(1 - sinTargetSqrt);
            cosDir = fabs(cosDir);
            double rs = ((refractiveIndexOfObject * cosDir) - (refractiveIndexOfRay * cosTarget)) / ((refractiveIndexOfObject * cosDir) + (refractiveIndexOfRay * cosTarget));
            double rp = ((refractiveIndexOfRay * cosDir) - (refractiveIndexOfObject * cosTarget)) / ((refractiveIndexOfRay * cosDir) + (refractiveIndexOfObject * cosTarget));

            return (rs * rs + rp * rp) / 2;
        }
    }

    static Vec3 hemisphere(Vec3 normal, std::mt19937 &rng) {
        std::uniform_real_distribution<> unit(0.0, 1.0);
        //Manually calculate tangent and bitangent based on the cross product of the vector (1, 0, 0) and the normal
        //and then the cross product of that vector and the normal
        //n x (1, 0, 0) = (n.y * 0 - n.z * 0, n.z * 1 - n.x * 0, n.x * 0 - n.y * 1)
        Vec3 tangent(abs(normal.x) == 1 ? Vec3(0, 0, 1) : Vec3(0, normal.z, -normal.y));
        Vec3 bitangent(abs(normal.x) == 1 ? Vec3(0, -1, 0) : Vec3(-normal.y * normal.y - normal.z * normal.z, normal.x * normal.y, normal.x * normal.z).normalize());

        double r = sqrt(unit(rng));
        double theta = unit(rng) * 2.0 * M_PI;

        double x = r * cos(theta);
        double y = r * sin(theta);
        double z = sqrt(1.0 - (x * x + y * y));

        return (normal * z + tangent * x + bitangent * y).normalize();
    }
};
