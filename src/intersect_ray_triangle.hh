// Adapted from https://github.com/rkruppe/watertri.rs
//
// An implementation of the ray-triangle intersection algorithm
// described in:
//
// > Sven Woop, Carsten Benthin, and Ingo Wald. "Watertight
// > ray/triangle intersection."  Journal of Computer Graphics
// > Techniques (JCGT) 2.1 (2013): 65-82.
//
// Does not perform backface culling.

#ifndef SRC_INTERSECT_RAY_TRIANGLE_HH_
#define SRC_INTERSECT_RAY_TRIANGLE_HH_

#include "glm/vec3.hpp"

#include "common.hh"

namespace mgrid {

class Ray3;

class Triangle {
 public:
  vec3 A, B, C;
};

class Hit {
 public:
  Hit(const vec3& barycentric, const float distance);

  vec3 barycentric;
  float distance;
};

// Precomputed data depending only on the ray
class RayData {
 public:
  static RayData from_ray(const Ray3& ray);

  optional<Hit> intersect_triangle(const Triangle& tri) const;

  int kx;
  int ky;
  int kz;
  float Sx;
  float Sy;
  float Sz;
  vec3 org;
};

optional<vec4> intersect_ray_triangle(const Ray3& ray, const Triangle& tri);
}

#endif  // SRC_INTERSECT_RAY_TRIANGLE_HH_
