#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

class Vec3 {
public:
    double x, y, z;
	Vec3() : x(0), y(0), z(0) {}
	Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3 operator*(double k) {
        return Vec3(x * k, y * k, z * k);
    }

    Vec3 operator*(Vec3 v) {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

	Vec3 operator+(Vec3 v) {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator+(double k) {
		return Vec3(x + k, y + k, z + k);
	}

	Vec3 operator-(Vec3 v) {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator-() {
		return Vec3(-x, -y, -z);
	}

    Vec3 operator*=(double k) {
        x *= k;
        y *= k;
        z *= k;

        return *this;
    }


    Vec3 operator*=(Vec3 v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;

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
    bool operator==(Vec3 v) {
        return x == v.x && y == v.y && z == v.z;
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
        double l = lengthSqrt();
        if (l == 1) return *this;

        l = sqrt(l);

		x /= l;
		y /= l;
		z /= l;

		return *this;
	}

	Vec3 normalized() {
	    double l = lengthSqrt();
	    if (l == 1) return Vec3(*this);

	    l = sqrt(l);

		return Vec3(x / l, y / l, z / l);
	}

	bool isZero() {
	    return x == 0 && y == 0 && z == 0;
	}

    friend std::ostream& operator<<(std::ostream &strm, const Vec3 &v);
};