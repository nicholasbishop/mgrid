#include "grid.hh"

#include "glm/vec2.hpp"

namespace mgrid {
Grid::Grid(const ivec2& resolution) {
  points_.resize(resolution.x * resolution.y);
}

Grid::~Grid() {}

bool intersect_ray(const Ray& UNUSED(ray)) {
  // TODO
  return false;
}
}
