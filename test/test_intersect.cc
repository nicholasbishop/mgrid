#include "catch.hpp"

#include "src/intersect_ray_triangle.hh"
#include "src/ray.hh"
#include "test/test_common.hh"

using namespace mgrid;

TEST_CASE("triangle intersect", "[intersection]") {
  Triangle tri{{0, 0, 0}, {100, 0, 0}, {0, 100, 0}};
  Ray3 ray1{{10, 10, 100}, {0, 0, -1}};
  Ray3 ray2{{0, 0, 100}, {0, 0, -1}};
  Ray3 ray3{{-10, -10, 100}, {0, 0, -1}};
  assert_equalish(*intersect_ray_triangle(ray1, tri),
                  vec4{0.8, 0.1, 0.1, 100});
  assert_equalish(*intersect_ray_triangle(ray2, tri),
                  vec4{1, 0, 0, 100});
  REQUIRE(intersect_ray_triangle(ray3, tri) == nullopt);
}
