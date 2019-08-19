#include "pch.h"
#include "Vec3.h"
#include "Sphere.h"
#include "Scene.h"
#include "Plane.h"
#include "Disk.h"
#include <iostream>
#include <cmath>
#include <cstdio>
#include <list>
#include <random>

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
const auto numSamples = 20000;

int main() {
	Scene scene;

	scene.add(std::make_unique<Disk>(Vec3(0, -3, 10), Vec3(0, 1, 0), 1.0, Material(Vec3(0.8, 0.2, 0.8), Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(-3, 0, 0), Vec3(1, 0, 0), Material::materialWithDiffusion(Vec3(1, 0, 0))));
    scene.add(std::make_unique<Plane>(Vec3(3, 0, 0), Vec3(-1, 0, 0), Material::materialWithDiffusion(Vec3(0, 1, 0))));
    scene.add(std::make_unique<Plane>(Vec3(0, 3, 0), Vec3(0, -1, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(0, -3, 0), Vec3(0, 1, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(0, 0, 20), Vec3(0, 0, -1), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(0, 0, -20), Vec3(0, 0, 1), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Sphere>(Vec3(1, 1.75, 13), 1.25, Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Sphere>(Vec3(-1, 1.75, 10), 1.25, Material::materialWithDiffusion(Vec3(1, 1, 1))));

    std::mt19937 rng(0);

	FILE *f = fopen("image.ppm", "w");
	fprintf(f, "P3\n%d %d\n%d\n", WIDTH, HEIGHT, 255);

	for (int y = 0; y < HEIGHT; y++) {
		auto yy = (static_cast<double>(y) / HEIGHT - 0.5) * 2;

		for (int x = 0; x < WIDTH; x++) {
			auto xx = (static_cast<double>(x) / WIDTH - 0.5) * 2;

			auto rayDir = (xCam * xx + yCam * yy + camDir).normalized();
			Ray ray(camPos, rayDir);

            Vec3 color(0, 0, 0);
			for (int i = 0; i < numSamples; i++) {
                color += scene.calculateColor(ray, rng);
			}

			color *= 1.0/numSamples;

			fprintf(f, "%i %i %i ", rgbToInt(color.x), rgbToInt(color.y), rgbToInt(color.z));
		}

		std::cout << ((double) y/HEIGHT) * 100.0 << "% done" << std::endl;
	}

	fclose(f);

	return 0;
}