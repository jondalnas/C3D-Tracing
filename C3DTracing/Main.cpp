#define USING_VISUAL_STUDIO true
#if USING_VISUAL_STUDIO
#include <Windows.h>
#endif

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
#include <functional>
#include <SDL.h>

namespace {
    uint8_t rgbToInt(double c) { return lround(pow(std::clamp(c, 0.0, 1.0), 1.0 / 2.2) * 255); }
}

constexpr unsigned int WINDOW_SLEEP_TIME = 100;

Vec3 camPos(0, 0, 0);
Vec3 camDir(0, 0, 1);

const int WIDTH = 640, HEIGHT = 480;
//const int WIDTH = 128, HEIGHT = 136;
const auto aspectRatio = static_cast<double>(WIDTH) / HEIGHT;
const auto xFOV = 60;
const auto yFOV = xFOV / aspectRatio;
Vec3 xCam(sin(xFOV * acos(-1) / 180), 0, 0); //TODO: Why doesn't it like const?
Vec3 yCam(0, sin(yFOV * acos(-1) / 180), 0);
//const auto numSamples = 5000;
const auto numSamples = 750;
std::chrono::time_point startTime = std::chrono::steady_clock::now();

int main(int argc, char* argv[]) {
	#if USING_VISUAL_STUDIO
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	#endif

    Scene scene;

    scene.add(std::make_shared<Disk>(Vec3(0, -3, 10), Vec3(0, 1, 0), 1.0, Material(Vec3(0.8, 0.2, 0.8), Vec3(1, 1, 1))));
    scene.add(std::make_shared<Plane>(Vec3(-3, 0, 0), Vec3(1, 0, 0), Material::materialWithDiffusion(Vec3(1, 0, 0))));
    scene.add(std::make_shared<Plane>(Vec3(3, 0, 0), Vec3(-1, 0, 0), Material::materialWithDiffusion(Vec3(0, 1, 0))));
    scene.add(std::make_shared<Plane>(Vec3(0, 3, 0), Vec3(0, -1, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_shared<Plane>(Vec3(0, -3, 0), Vec3(0, 1, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_shared<Plane>(Vec3(0, 0, 20), Vec3(0, 0, -1), Material::materialWithDiffusion(Vec3(1, 1, 1))));
    scene.add(std::make_shared<Plane>(Vec3(0, 0, -1), Vec3(0, 0, 1), Material::materialWithDiffusion(Vec3(1, 1, 1))));
	//scene.add(std::make_shared<Sphere>(Vec3(-5, 1.75, 10), 1.25, Material(Vec3(0.8314, 0.6863, 0.2157), 2, 1.0 / 6.0)));
	//scene.add(std::make_shared<Sphere>(Vec3(-3, 1.75, 15), 1.25, Material(Vec3(0.8314, 0.6863, 0.2157), 2, 2.0 / 6.0)));
	//scene.add(std::make_shared<Sphere>(Vec3(-1, 1.75, 10), 1.25, Material(Vec3(0.8314, 0.6863, 0.2157), 2, 3.0 / 6.0)));
	scene.add(std::make_shared<Sphere>(Vec3(1, 1.75, 10), 1.25, Material(Vec3(0.8314, 0.6863, 0.2157), 2, 2.0 / 6.0)));
	//scene.add(std::make_shared<Sphere>(Vec3(3, 1.75, 10), 1.25, Material(Vec3(0.8314, 0.6863, 0.2157), 2, 5.0 / 6.0)));
	//scene.add(std::make_shared<Sphere>(Vec3(5, 1.75, 15), 1.25, Material(Vec3(0.8314, 0.6863, 0.2157), 2, 6.0 / 6.0)));
    //scene.add(std::make_shared<Sphere>(Vec3(0, 1.75, 6), 1.25, Material::refractiveMaterial(1.333) + Material::reflectiveMaterial()));//Material(Vec3(1, 1, 1), 100, 10)));

    /*scene.add(std::make_shared<Disk>(Vec3(3, 0, 7), Vec3(-1, 0, 0), 1.0, Material(Vec3(0.8, 0.2, 0.8), Vec3(1, 1, 1))));
    scene.add(std::make_shared<Sphere>(Vec3(0, 0, 7), 1, Material::refractiveMaterial(1.333)));
    scene.add(std::make_shared<Plane>(Vec3(-2.01515152, 0, 0), Vec3(1, 0, 0), Material::materialWithDiffusion(Vec3(1, 1, 1))));*/

    startTime = std::chrono::steady_clock::now();

    std::vector<Vec3> _output;
    _output.reserve(WIDTH * HEIGHT);
    for (auto i = 0u; i < WIDTH * HEIGHT; i++) {
        _output.emplace_back(Vec3());
    }

    struct Pixel {
    public:
        unsigned int x, y;

        Pixel(unsigned int x, unsigned int y) : x(x), y(y) {
        }

        Pixel() : x(0), y(0) {}
    };

    struct Pixels {
	private:
		std::mutex nextMutex;
		std::mutex cueMutex;

		std::vector<Pixel> pixels;
		std::vector<Pixel> closed;
		std::vector<Pixel> canRender;

    public:
        int widht, height;

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
            std::unique_lock lock(nextMutex);

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

		bool noPixels() {
			std::unique_lock lock(nextMutex);

			bool empty = pixels.size() == 0;

			lock.unlock();

			return empty;
		}

		void addToRenderCue(Pixel pixel) {
			std::unique_lock lock(cueMutex);

			canRender.emplace_back(pixel);

			lock.unlock();
		}

		Pixel getNextPixelInCue() {
			std::unique_lock lock(cueMutex);

			Pixel pixel = canRender.front();

			canRender.erase(canRender.begin());

			lock.unlock();
			return pixel;
		}

		bool hasNextPixelInCue() {
			std::unique_lock lock(cueMutex);

			bool empty = canRender.size() == 0;

			lock.unlock();

			return !empty;
		}
    };

    Pixels pixelArray(WIDTH, HEIGHT);

    std::mt19937 rng(0);

    std::vector<std::thread> threads;
	threads.reserve(std::thread::hardware_concurrency() + 1);
	for (auto i = 0u; i < std::thread::hardware_concurrency(); i++) {
        threads.emplace_back([&] {
            std::pair<Pixel, bool> pixel;
            while ((pixel = pixelArray.next()).second) {
				auto xx = (static_cast<double>(pixel.first.x) / WIDTH - 0.5) * 2;
                auto yy = (static_cast<double>(pixel.first.y) / HEIGHT - 0.5) * 2;

                auto rayDir = (xCam * xx + yCam * yy + camDir).normalized();
                Ray ray(camPos, rayDir);

                std::pair<Geometry::Hit, bool> closest = scene.findGeometry(ray);

                for (int i = 0; i < numSamples; i++) {
                    Vec3 color = scene.calculateColor(ray, rng, closest);

                    _output[pixel.first.x + pixel.first.y * WIDTH] += color;
                }

                pixelArray.addToRenderCue(pixel.first);
            }
        });
    }

    threads.emplace_back([&]{
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Init(SDL_INIT_VIDEO);

        SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_OPENGL, &window, &renderer);
        SDL_SetWindowTitle(window, "C3D Tracer");

		SDL_Event evt;

        while (!pixelArray.noPixels()) {
			SDL_Delay(WINDOW_SLEEP_TIME);

			SDL_PollEvent(&evt);

			if (evt.type == SDL_QUIT) {
				for (auto &thread : threads) {
					thread.detach();

					thread.~thread();
				}
			}

			while (pixelArray.hasNextPixelInCue()) {
				Pixel currentPixel = pixelArray.getNextPixelInCue();

				auto c = _output[currentPixel.x + currentPixel.y * WIDTH];

				c *= 1.0 / numSamples;

				SDL_SetRenderDrawColor(renderer, rgbToInt(c.x), rgbToInt(c.y), rgbToInt(c.z), 255);

				SDL_RenderDrawPoint(renderer, currentPixel.x, currentPixel.y);
			}

            SDL_RenderPresent(renderer);
        }

        SDL_DestroyWindow(window);
        SDL_Quit();
    });

    for (auto &thread : threads) {
        thread.join();
    }

    FILE *f = fopen("BRDF.ppm", "w");
    fprintf(f, "P3\n%d %d\n%d\n", WIDTH, HEIGHT, 255);

    for (auto c : _output) {
        c *= 1.0 / numSamples;

        fprintf(f, "%i %i %i ", rgbToInt(c.x), rgbToInt(c.y), rgbToInt(c.z));
    }

    fclose(f);

    return 0;
}