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

  optional<vec4> intersect_ray(const Ray3& ray);

  vec3* data() { return points_.data(); }

  template <typename F>
  void triangle_iter(F f) {
    const std::size_t w = res_.x;
    const std::size_t h = res_.y;

    for (std::size_t y = 1; y < h; y++) {
      for (std::size_t x = 1; x < w; x++) {
        const std::size_t ind[4] = {
            // clang-format off
            (y - 1) * w + (x - 1),
            (y - 1) * w + (x - 0),
            (y - 0) * w + (x - 1),
            (y - 0) * w + (x - 0),
            // clang-format on
        };
        const std::size_t pattern[2][3] = {
            // clang-format off
            {0, 2, 1},
            {0, 3, 2}
            // clang-format on
        };
        for (int t = 0; t < 2; t++) {
          f(std::array<std::size_t, 3>{ind[pattern[t][0]], ind[pattern[t][1]],
                                       ind[pattern[t][2]]});
        }
      }
    }
  }

 private:
  std::vector<vec3> points_;
  ivec2 res_;
};
}

#endif  // SRC_GRID_HH_
