#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "common.hh"
#include "intersect_ray_triangle.hh"

namespace mgrid {

class Grid {
 public:
  Grid();

  ~Grid();

  const ivec2& res() const { return res_; }

  void make_random(const ivec2& res);

  optional<vec4> intersect_ray(const Ray3& ray);

  vec3* data() { return points_.data(); }

 private:
  std::vector<vec3> points_;
  ivec2 res_;
};
}

#endif  // SRC_GRID_HH_
