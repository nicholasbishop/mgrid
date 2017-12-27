cmake_minimum_required(VERSION 3.0)
project(mgrid)

find_package(glfw3 3.2 REQUIRED)

add_compile_options(-Wall -Wextra)
include_directories(.)

add_executable(mgrid src/main.cc)

target_link_libraries(mgrid glfw3)
