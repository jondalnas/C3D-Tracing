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
#include <chrono>
#include <thread>
#include <mutex>

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
std::chrono::time_point startTime = std::chrono::steady_clock::now();

int main() {
    Scene scene;

    scene.add(std::make_unique<Disk>(Vec3(0, -3, 10), Vec3(0, 1, 0), 1.0, Material(Vec3(0.8, 0.2, 0.8), Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(-3, 0, 0), Vec3(1, 0, 0), Material::materialWithDiffusion(Vec3(1, 0, 0))));
    scene.add(std::make_unique<Plane>(Vec3(3, 0, 0), Vec3(-1, 0, 0), Material::materialWithDiffusion(Vec3(0, 1, 0))));
    scene.add(std::make_unique<Plane>(Vec3(0, 3, 0), Vec3(0, -1, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(0, -3, 0), Vec3(0, 1, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(0, 0, 20), Vec3(0, 0, -1), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Plane>(Vec3(0, 0, -20), Vec3(0, 0, 1), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_unique<Sphere>(Vec3(1, 1.75, 13), 1.25, Material(Vec3(1, 1, 1), 100, 10)));
    scene.add(std::make_unique<Sphere>(Vec3(-1, 1.75, 10), 1.25, Material(Vec3(1, 1, 1), 100, 10)));

    startTime = std::chrono::steady_clock::now();

    std::vector<Vec3> _output;
    _output.reserve(WIDTH * HEIGHT);
    for (auto i = 0u; i < WIDTH * HEIGHT; i++) {
        _output.emplace_back(Vec3());
    }

    struct Pixel {
    public:
        int x, y;

        Pixel(int x, int y) : x(x), y(y) {
        }

        Pixel() {}
    };

    struct Pixels {
    public:
        int widht, height;
        std::mutex mutex;

        std::vector<Pixel> pixels;
        std::vector<Pixel> closed;
        Pixels(int width, int height) : widht(width), height(height) {
            pixels.reserve(width * height);
            closed.reserve(width * height);

            for (int y = 0; y  < height; y++) {
                for (int x = 0; x < width; x++) {
                    pixels.emplace_back(Pixel(x, y));
                }
            }
        }

        std::pair<Pixel, bool> next() {
            std::unique_lock lock(mutex);

            if (pixels.empty())
                return {};

            auto pixel = pixels.back();

            closed.emplace_back(std::move(pixel));
            pixels.pop_back();

            if ((pixels.size() & 255) == 0) {
                double deltaTime = (std::chrono::steady_clock::now() - startTime).count() / 1e9;
                std::cout << (1.0 - (pixels.size() / (float) (WIDTH * HEIGHT))) * 100.0 << "%" << ", ETA "
                          << (deltaTime / closed.size()) *
                             pixels.size() << " seconds" << std::endl;
            }

            lock.unlock();

            return {pixel, true};
        }
    };

    Pixels pixelArray(WIDTH, HEIGHT);

    std::mt19937 rng(0);

    std::vector<std::thread> threads;
    threads.reserve(std::thread::hardware_concurrency());
    for (auto i = 0u; i < std::thread::hardware_concurrency(); i++) {
        threads.emplace_back([&] {
            std::pair<Pixel, bool> pixel;
            while ((pixel = pixelArray.next()).second) {
                auto xx = (static_cast<double>(pixel.first.x) / WIDTH - 0.5) * 2;
                auto yy = (static_cast<double>(pixel.first.y) / HEIGHT - 0.5) * 2;

                auto rayDir = (xCam * xx + yCam * yy + camDir).normalized();
                Ray ray(camPos, rayDir);

                for (int i = 0; i < numSamples; i++) {
                    Vec3 color = scene.calculateColor(ray, rng);
                    color *= 1.0 / numSamples;
                    _output[pixel.first.x + pixel.first.y * WIDTH] += color;
                }
            }
        });
    }

    for (auto &thread : threads) {
        thread.join();
    }

    FILE *f = fopen("image.ppm", "w");
    fprintf(f, "P3\n%d %d\n%d\n", WIDTH, HEIGHT, 255);

    for (auto c : _output) {
        fprintf(f, "%i %i %i ", rgbToInt(c.x), rgbToInt(c.y), rgbToInt(c.z));
    }

    fclose(f);

    return 0;
}