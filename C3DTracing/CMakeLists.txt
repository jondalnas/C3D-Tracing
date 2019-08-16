cmake_minimum_required(VERSION 3.6)
project(C3DTracing)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Werror -Wextra -march=native -g)

file(GLOB source *.cpp *.h)
add_executable(C3DTracing ${source} Scene.h Scene.cpp Material.h)
