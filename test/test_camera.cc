#include "catch.hpp"

#include "src/camera.hh"
#include "test/test_common.hh"

using namespace mgrid;

TEST_CASE("ray generation", "[camera]") {
  Camera cam;
  cam.set_distance(2);
  cam.set_size({2, 2});
  const auto ray = cam.ray({1, 1});
  assert_equalish(ray.direction, vec3{0, 0, -1});
}
