#ifndef SRC_INTERSECT_RAY_TRIANGLE_HH_
#define SRC_INTERSECT_RAY_TRIANGLE_HH_

#include "glm/vec3.hpp"

#include "common.hh"

namespace mgrid {

class Ray {
 public:
  const vec3 origin;
  const vec3 direction;
};

class Triangle {
 public:
  vec3 A, B, C;
};

optional<vec4> intersect_ray_triangle(const Ray& ray, const Triangle& tri);
}

#endif  // SRC_INTERSECT_RAY_TRIANGLE_HH_
