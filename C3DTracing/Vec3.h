#pragma once
#include <cmath>

class Vec3 {
public:
	double x, y, z;
	Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

	Vec3 operator*(double k) {
		return Vec3(x * k, y * k, z * k);
	}

	Vec3 operator+(Vec3 v) {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator-(Vec3 v) {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator*=(double k) {
		x *= k;
		y *= k;
		z *= k;

		return *this;
	}

	Vec3 operator+=(Vec3 v) {
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	Vec3 operator-=(Vec3 v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	double dot(Vec3 v) {
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 cross(Vec3 v) {
		return Vec3(y * v.z - z * v.y,
					z * v.x - x * v.z,
					x * v.y - y * v.x);
	}

	double length() {
		return sqrt(x * x + y * y + z * z);
	}

	double lengthSqrt() {
		return x * x + y * y + z * z;
	}

	Vec3 normalize() {
		double l = length();
		x /= l;
		y /= l;
		z /= l;

		return *this;
	}

	Vec3 normalized() {
		double l = length();

		return Vec3(x / l, y / l, z / l);
	}
};

