#include "pch.h"
#include "Sphere.h"
#include <cmath>
#include <iostream>

bool Sphere::intersects(Ray ray) {
	Vec3 toSphere = pos - ray.pos;
	double distanceSqrt = toSphere.lengthSqrt();

	double centerHit = ray.dir.dot(toSphere);
	if (centerHit < 0) return false;

	double closestDistance = sqrt(distanceSqrt - centerHit * centerHit);
	if (closestDistance > radius) return false;

	return true;
}