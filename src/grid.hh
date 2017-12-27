#ifndef SRC_GRID_HH_
#define SRC_GRID_HH_

#include <vector>

#include "glm/vec3.hpp"

#include "common.hh"
#include "intersect_ray_triangle.hh"

namespace mgrid {

class Grid {
 public:
  Grid(const ivec2& resolution);

  ~Grid();

  bool intersect_ray(const Ray& ray);

 private:
  std::vector<vec3> points_;
};
}

#endif  // SRC_GRID_HH_
