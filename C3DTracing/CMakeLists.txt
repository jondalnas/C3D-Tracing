cmake_minimum_required(VERSION 3.6)
project(C3DTracing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/opt/clion/bin/cmake/linux/share/cmake-3.14/Modules")

add_compile_options(-Wall -Werror -Wextra -march=native -g)

find_package(SDL2 REQUIRED)

file(GLOB source *.cpp *.h)
add_executable(C3DTracing ${source} Geometry.h Scene.h Scene.cpp Material.h Plane.cpp Plane.h Disk.cpp Disk.h Math.cpp Math.h Ray.h)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(C3DTracing ${SDL2_LIBRARIES})