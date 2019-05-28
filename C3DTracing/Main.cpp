#define _USE_MATH_DEFINES

#include "pch.h"
#include "Vec3.h"
#include "Sphere.h"
#include <iostream>
#include <cmath>

Vec3 camPos(0, 0, 0);
Vec3 camDir(0, 0, 1);

const int WIDTH = 64, HEIGHT = 48;
const auto aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
const auto xFOV = 60;
const auto yFOV = xFOV / aspectRatio;
Vec3 xCam(sin(xFOV * acos(-1) / 180), 0, 0); //TODO: Why doesn't it like const?
Vec3 yCam(0, sin(yFOV * acos(-1) / 180), 0);

int main() {
	Sphere s(Vec3(0, 0, 10), 3);

	std::cout <<  sin(60) << std::endl;

	for (int y = 0; y < HEIGHT; y++) {
		auto yy = (static_cast<double>(y) / HEIGHT - 0.5) * 2;

		for (int x = 0; x < WIDTH; x++) {
			auto xx = (static_cast<double>(x) / WIDTH - 0.5) * 2;

			auto rayDir = (xCam * xx + yCam * yy + camDir).normalized();
			Ray ray(camPos, rayDir);

			bool hit = s.intersects(ray);
			if (hit) {
				std::cout << '#';
			}
			else {
				std::cout << ' ';
			}
		}

		std::cout << std::endl;
	}

	return 0;
}