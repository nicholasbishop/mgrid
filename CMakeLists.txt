cmake_minimum_required(VERSION 3.0)
project(mgrid)

include(infra/CMakeRC.cmake)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
find_package(glfw3 3.2 REQUIRED)
pkg_check_modules(LIBEPOXY epoxy REQUIRED)
pkg_check_modules(eigen3 REQUIRED)

add_compile_options(-Wall -Wextra)
include_directories(. glm)

cmrc_add_resource_library(resources
  src/shaders/grid_frag.glsl
  src/shaders/grid_geom.glsl
  src/shaders/grid_tess_ctrl.glsl
  src/shaders/grid_tess_eval.glsl
  src/shaders/grid_vert.glsl
  )

add_executable(mgrid
  src/angle.cc
  src/camera.cc
  src/errors.cc
  src/main.cc
  src/shader.cc
  src/texture.cc
  src/window.cc)

target_link_libraries(mgrid ${LIBEPOXY_LIBRARIES} glfw resources)
