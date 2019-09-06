#include "pch.h"
#include "Sphere.h"
#include <cmath>
#include <iostream>

std::pair<Geometry::Hit, bool> Sphere::intersects(Ray ray) {
	Vec3 toSphere = pos - ray.pos;
	double distanceSqrt = toSphere.lengthSqrt();

	double centerHit = ray.dir.normalized().dot(toSphere);
	if (centerHit < 0) return{};

	double closestDistanceSqrt = distanceSqrt - centerHit * centerHit;
	if (closestDistanceSqrt < 0 || closestDistanceSqrt > radius * radius) return{};

	double t = sqrt(radius * radius - closestDistanceSqrt);
	double minusT = centerHit - t;
	double plusT = centerHit + t;

	if (minusT > plusT) std::swap(plusT, minusT);

	if (minusT < 1e-4) {
	    minusT = plusT;
	    if (plusT < 1e-4) return {};
	}

	Vec3 hitPos = ray.dir * minusT + ray.pos;

    return { Hit{ minusT, hitPos, (hitPos - pos).normalized(), mat, this }, true };
}

bool Sphere::inside(Vec3 point) {
    return (point - pos).lengthSqrt() <= radius * radius + 1e-2;
}