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

	if (minusT > plusT) {
		std::swap(plusT, minusT);
	}

	Vec3 hitPos = ray.dir * minusT;

    return { Hit{ minusT, hitPos, (hitPos - pos).normalized(), mat }, true };
}