cmake_minimum_required(VERSION 3.0)
project(mgrid)

find_package(PkgConfig REQUIRED)
find_package(glfw3 3.2 REQUIRED)
pkg_check_modules(LIBEPOXY epoxy REQUIRED)
pkg_check_modules(eigen3 REQUIRED)

add_compile_options(-Wall -Wextra)
include_directories(. glm)

add_executable(mgrid
  src/angle.cc
  src/camera.cc
  src/main.cc
  src/shader.cc
  src/window.cc)

target_link_libraries(mgrid ${LIBEPOXY_LIBRARIES} glfw)
