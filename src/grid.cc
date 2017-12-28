#include "grid.hh"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include "glm/gtx/intersect.hpp"

#include "intersect_ray_triangle.hh"

namespace mgrid {
Grid::Grid() {}

Grid::~Grid() {}

void Grid::make_random(const ivec2& res) {
  const float hw = res.x / 2.0f;
  const float hh = res.y / 2.0f;
  points_.resize(res.x * res.y);
  res_ = res;
  for (int y = 0; y < res.y; y++) {
    for (int x = 0; x < res.x; x++) {
      auto& point = points_[y * res.x + x];
      point.x = x / hw - 1;
      point.y = y / hh - 1;
      point.z = ((rand() % 128) - 64) / 1024.0f;
    }
  }
}

optional<vec4> Grid::intersect_ray(const Ray3& ray) {
  const int w = res_.x;
  const int h = res_.y;

  optional<vec4> hit;

  for (int y = 1; y < h; y++) {
    for (int x = 1; x < w; x++) {
      const int ind[4] = {
          // clang-format off
          (y - 1) * w + (x - 1),
          (y - 1) * w + (x - 0),
          (y - 0) * w + (x - 0),
          (y - 0) * w + (x - 1),
          // clang-format on
      };
      const int pattern[2][3] = {{0, 1, 2}, {0, 2, 3}};
      for (int t = 0; t < 2; t++) {
        Triangle tri{points_[ind[pattern[t][0]]], points_[ind[pattern[t][1]]],
                     points_[ind[pattern[t][2]]]};

        //   GLM_FUNC_DECL bool intersectRayTriangle(
        // vec<3, T, Q> const& orig, vec<3, T, Q> const& dir,
        // vec<3, T, Q> const& v0, vec<3, T, Q> const& v1, vec<3, T, Q> const&
        // v2,
        // vec<3, T, Q>& baryPosition, T& distance);

        const auto res = intersect_ray_triangle(ray, tri);
        if (res && hit && res->w < hit->w) {
          hit = res;
        }
      }
    }
  }

  return hit;
}
}
