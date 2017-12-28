#include "grid.hh"

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/intersect.hpp"

#include "intersect_ray_triangle.hh"
#include "ray.hh"

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

optional<Hit> Grid::intersect_ray(const Ray3& ray) {
  optional<Hit> hit;

  const auto ray_data = RayData::from_ray(ray);

  triangle_iter([&](const std::array<std::size_t, 3>& ind) {
    Triangle tri{points_[ind[0]], points_[ind[1]], points_[ind[2]]};

    const auto new_hit{ray_data.intersect_triangle(tri)};
    if (new_hit) {
      if (hit && new_hit->distance < hit->distance) {
        hit = new_hit;
      } else if (!hit) {
        hit = new_hit;
      }
    }

    // if (glm::intersectRayTriangle(ray.origin, ray.direction, tri.A, tri.B,
    //                               tri.C, bary, dist)) {
    //   if ((!hit) || (dist < hit->w)) {
    //     hit = vec4(bary, 1.0f - bary.x - bary.y, dist);
    //   }
    // }
  });

  return hit;
}
}
