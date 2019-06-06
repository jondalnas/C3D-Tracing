#include "pch.h"
#include "Sphere.h"
#include <cmath>
#include <iostream>

std::pair<Geometry::Hit, bool> Sphere::intersects(Ray ray) {
	Vec3 toSphere = pos - ray.pos;
	double distanceSqrt = toSphere.lengthSqrt();

	double centerHit = ray.dir.dot(toSphere);
	if (centerHit < 0) return{};

	double closestDistanceSqrt = distanceSqrt - centerHit * centerHit;
	if (closestDistanceSqrt > radius * radius) return{};

	double t = sqrt(radius * radius - closestDistanceSqrt);
	double minusT = centerHit - t;
	double plusT = centerHit + t;

	if (minusT > plusT) {
		plusT = minusT;
	}

	Vec3 hitPos = ray.dir * plusT;

	return { Hit{ plusT, hitPos, (hitPos - pos).normalized(), mat }, true };
}