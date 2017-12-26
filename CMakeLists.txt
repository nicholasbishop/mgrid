cmake_minimum_required(VERSION 3.0)
project(mgrid)

set(CMAKE_AUTORCC on)

find_package(Qt53DExtras)

add_compile_options(-Wall -Wextra)
include_directories(.)

add_executable(mgrid
  src/grid.cc
  src/main.cc
  src/resources.qrc)

target_link_libraries(mgrid Qt5::3DExtras)
