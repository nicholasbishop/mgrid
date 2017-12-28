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
  src/shaders/basic.frag.glsl
  src/shaders/basic.vert.glsl
  src/shaders/grid_frag.glsl
  src/shaders/grid_geom.glsl
  src/shaders/grid_tess_ctrl.glsl
  src/shaders/grid_tess_eval.glsl
  src/shaders/grid_vert.glsl)

add_library(internal
  src/angle.cc
  src/camera.cc
  src/draw_mesh.cc
  src/errors.cc
  src/events.cc
  src/grid.cc
  src/intersect_ray_triangle.cc
  src/shader.cc
  src/texture.cc
  src/vao.cc
  src/window.cc)

add_executable(mgrid src/main.cc)

add_executable(tests
  test/test_camera.cc
  test/test_intersect.cc
  test/test_main.cc)

target_link_libraries(internal ${LIBEPOXY_LIBRARIES} glfw resources)

target_link_libraries(mgrid internal)
target_link_libraries(tests internal)
