#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <array>
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

#include "common.hh"
#include "intersect_ray_triangle.hh"

namespace mgrid {

class Grid {
 public:
  Grid();

  Grid(const ivec2& res);

  ~Grid();

  const ivec2& res() const { return res_; }

  void make_random(const ivec2& res);

  optional<Hit> intersect_ray(const Ray3& ray);

  vec3* data() { return points_.data(); }

  template <typename F>
  void points_iter(F f);

  template <typename F>
  void triangle_iter(F f);

 private:
  std::vector<vec3> points_;
  ivec2 res_;
};
}

#include "grid_inline.hh"

#endif  // SRC_GRID_HH_
