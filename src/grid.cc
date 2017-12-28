#include "grid.hh"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include "glm/gtx/intersect.hpp"

#include "intersect_ray_triangle.hh"

namespace mgrid {

Grid::Grid() {}

Grid::Grid(const ivec2& res) : res_(res) {
  points_.resize(res.x * res.y);
}

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
  optional<vec4> hit;

  triangle_iter([this, &ray, &hit](const std::array<std::size_t, 3>& ind) {
    Triangle tri{points_[ind[0]], points_[ind[1]], points_[ind[2]]};

    //   GLM_FUNC_DECL bool intersectRayTriangle(
    // vec<3, T, Q> const& orig, vec<3, T, Q> const& dir,
    // vec<3, T, Q> const& v0, vec<3, T, Q> const& v1, vec<3, T, Q> const&
    // v2,
    // vec<3, T, Q>& baryPosition, T& distance);

    const auto res = intersect_ray_triangle(ray, tri);
    if (res && hit && res->w < hit->w) {
      hit = res;
    }
  });

  return hit;
}
}
