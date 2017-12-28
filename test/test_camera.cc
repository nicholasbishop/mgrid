#include "catch.hpp"

#include "src/camera.hh"
#include "test/test_common.hh"

using namespace mgrid;

TEST_CASE("unproject", "[camera]") {
  Camera cam;
  cam.set_distance(2);
  cam.set_size({2, 2});
  const auto p = cam.unproject({1, 1, 0});
  // 1.9 := distance - near
  assert_equalish(p, vec3{0, 0, 1.9});
}
