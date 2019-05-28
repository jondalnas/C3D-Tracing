#include "pch.h"
#include "Vec3.h"
#include "Sphere.h"
#include <iostream>
#include <cmath>
#include <cstdio>

namespace {
	double clamp(double v, double min, double max) { return v < min ? min : (v > max ? max : v); }

	int rgbToInt(double c) { return int(pow(clamp(c, 0.0, 1.0), 1.0 / 2.2) * 255 + 0.5); }
}

Vec3 camPos(0, 0, 0);
Vec3 camDir(0, 0, 1);

const int WIDTH = 640, HEIGHT = 480;
const auto aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
const auto xFOV = 60;
const auto yFOV = xFOV / aspectRatio;
Vec3 xCam(sin(xFOV * acos(-1) / 180), 0, 0); //TODO: Why doesn't it like const?
Vec3 yCam(0, sin(yFOV * acos(-1) / 180), 0);

int main() {
	Sphere s(Vec3(0, 0, 10), 3);

	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", WIDTH, HEIGHT, 255);

	for (int y = 0; y < HEIGHT; y++) {
		auto yy = (static_cast<double>(y) / HEIGHT - 0.5) * 2;

		for (int x = 0; x < WIDTH; x++) {
			auto xx = (static_cast<double>(x) / WIDTH - 0.5) * 2;

			auto rayDir = (xCam * xx + yCam * yy + camDir).normalized();
			Ray ray(camPos, rayDir);

			auto hit = s.intersects(ray);

			Vec3 col = Vec3();

			if (hit.second) {
				col = Vec3(1, 0, 1) * pow(rayDir.dot(hit.first.normal), 2);
			}

			fprintf(f, "%i %i %i ", rgbToInt(col.x), rgbToInt(col.y), rgbToInt(col.z));
		}
	}

	fclose(f);

	return 0;
}