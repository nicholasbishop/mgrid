cmake_minimum_required(VERSION 3.0)
project(mgrid)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-Wall -Wextra)
include_directories(. glm)

add_executable(mgrid src/main.cc src/mesh.cc)
