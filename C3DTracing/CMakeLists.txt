cmake_minimum_required(VERSION 3.6)
project(C3DTracing)

set(SDL2_PATH "C:\\Program Files (x86)\\SDL2\\i686-w64-mingw32")
set(SDL2_LIBRARY "C:\\Program Files (x86)\\SDL2\\i686-w64-mingw32\\lib")

find_package(SDL2 REQUIRED)

file(GLOB source *.cpp *.h)
add_executable(C3DTracing ${source} Geometry.h Scene.h Scene.cpp Material.h Plane.cpp Plane.h Disk.cpp Disk.h Math.cpp Math.h Ray.h)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(C3DTracing ${SDL2_LIBRARIES})