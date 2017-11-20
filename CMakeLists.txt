cmake_minimum_required(VERSION 3.0)
project(mgrid)


find_package(Qt53DExtras)

add_compile_options(-Wall -Wextra)

add_executable(mgrid src/main.cc)

target_link_libraries(mgrid Qt5::3DExtras)
