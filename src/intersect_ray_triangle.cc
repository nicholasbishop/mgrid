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

#include "intersect_ray_triangle.hh"

#include "glm/glm.hpp"

#include "ray.hh"

namespace mgrid {

int max_dim(const vec3& v) {
  const auto x = fabsf(v.x);
  const auto y = fabsf(v.y);
  const auto z = fabsf(v.z);

  if (x > y) {
    // y isn't the maximum, so it's either x or z
    if (x > z) {
      return 0;
    } else {
      return 2;
    }
  } else if (y > z) {
    // x isn't the maximum, so it's either y or z
    return 1;
  } else {
    return 2;
  }
}

RayData RayData::from_ray(const Ray3& src) {
  // The paper swaps kx and ky if dir[kz] is negative, to preserve
  // winding order. But winding order is only relevant for backface
  // culling, which we don't perform.
  RayData dst;
  dst.kz = max_dim(src.direction);
  dst.kx = (dst.kz + 1) % 3;
  dst.ky = (dst.kz + 2) % 3;
  dst.Sx = src.direction[dst.kx] / src.direction[dst.kz];
  dst.Sy = src.direction[dst.ky] / src.direction[dst.kz];
  dst.Sz = 1.0 / src.direction[dst.kz];
  dst.org = src.origin;
  return dst;
}

Hit::Hit(const vec3& barycentric, const float distance)
    : barycentric(barycentric), distance(distance) {}

double to_double(const float f) {
  return static_cast<double>(f);
}

float to_float(const double f) {
  return static_cast<float>(f);
}

// Perform the intersection calculation.
optional<Hit> RayData::intersect_triangle(const Triangle& tri) const {
  const auto A = tri.A - org;
  const auto B = tri.B - org;
  const auto C = tri.C - org;
  
  const auto Ax = tri.A[kx] - Sx * A[kz];
  const auto Ay = tri.A[ky] - Sy * A[kz];
  const auto Bx = tri.B[kx] - Sx * B[kz];
  const auto By = tri.B[ky] - Sy * B[kz];
  const auto Cx = tri.C[kx] - Sx * C[kz];
  const auto Cy = tri.C[ky] - Sy * C[kz];

  auto U = Cx * By - Cy * Bx;
  auto V = Ax * Cy - Ay * Cx;
  auto W = Bx * Ay - By * Ax;

  if (U == 0. || V == 0. || W == 0.) {
    const auto CxBy = to_double(Cx) * to_double(By);
    const auto CyBx = to_double(Cy) * to_double(Bx);
    U = to_float(CxBy - CyBx);
    const auto AxCy = to_double(Ax) * to_double(Cy);
    const auto AyCx = to_double(Ay) * to_double(Cx);
    V = to_float(AxCy - AyCx);
    const auto BxAy = to_double(Bx) * to_double(Ay);
    const auto ByAx = to_double(By) * to_double(Ax);
    W = to_float(BxAy - ByAx);
  }

  if ((U < 0. || V < 0. || W < 0.) && (U > 0. || V > 0. || W > 0.)) {
    return nullopt;
  }

  const auto det = U + V + W;
  if (det == 0.) {
    return nullopt;
  }

  const auto Az = Sz * A[kz];
  const auto Bz = Sz * B[kz];
  const auto Cz = Sz * C[kz];
  const auto T = U * Az + V * Bz + W * Cz;

  const auto rcpDet = 1.0f / det;
  return Hit{{U * rcpDet, V * rcpDet, W * rcpDet}, T * rcpDet};
}


uint32_t sign_mask(const float val) {
  if (val < 0.0f) {
    return 0xffffffff;
  } else {
    return 0x00000000;
  }
}

float xorf(const float f, const uint32_t mask) {
  float ret = 0;
  const auto* src1 = reinterpret_cast<const uint8_t*>(&f);
  const auto* src2 = reinterpret_cast<const uint8_t*>(&mask);
  auto* dst = reinterpret_cast<uint8_t*>(&ret);

  for (int i = 0; i < 4; i++) {
    dst[i] = src1[0] ^ src2[0];
  }

  return ret;
}

// Adapted from http://jcgt.org/published/0002/01/05/paper.pdf
optional<vec4> intersect_ray_triangle(const Ray3& ray, const Triangle& tri) {
  // TODO(nicholasbishop): move the precalculation out */

  /* calculate dimension where the is maximal */
  int kz = max_dim(glm::abs(ray.direction));
  int kx = kz + 1;
  if (kx == 3)
    kx = 0;
  int ky = kx + 1;
  if (ky == 3)
    ky = 0;
  // swap kx and ky dimension to preserve winding direction of
  // triangles
  if (ray.direction[kz] < 0.0f)
    std::swap(kx, ky);
  /* calculate shear constants */
  float Sx = ray.direction[kx] / ray.direction[kz];
  float Sy = ray.direction[ky] / ray.direction[kz];
  float Sz = 1.0f / ray.direction[kz];

  // TODO(nicholasbishop): end of precalculation

  /* calculate vertices relative to ray origin */
  const vec3 A = tri.A - ray.origin;
  const vec3 B = tri.B - ray.origin;
  const vec3 C = tri.C - ray.origin;

  /* perform shear and scale of vertices */
  const float Ax = A[kx] - Sx * A[kz];
  const float Ay = A[ky] - Sy * A[kz];
  const float Bx = B[kx] - Sx * B[kz];
  const float By = B[ky] - Sy * B[kz];
  const float Cx = C[kx] - Sx * C[kz];
  const float Cy = C[ky] - Sy * C[kz];

  /* calculate scaled barycentric coordinates */
  float U = Cx * By - Cy * Bx;
  float V = Ax * Cy - Ay * Cx;
  float W = Bx * Ay - By * Ax;
  /* fallback to test against edges using double precision */
  if (U == 0.0f || V == 0.0f || W == 0.0f) {
    double CxBy = (double)Cx * (double)By;
    double CyBx = (double)Cy * (double)Bx;
    U = (float)(CxBy - CyBx);
    double AxCy = (double)Ax * (double)Cy;
    double AyCx = (double)Ay * (double)Cx;
    V = (float)(AxCy - AyCx);
    double BxAy = (double)Bx * (double)Ay;
    double ByAx = (double)By * (double)Ax;
    W = (float)(BxAy - ByAx);
  }

// Perform edge tests. Moving this test before and at the end of the
// previous conditional gives higher performance.
#ifdef BACKFACE_CULLING
  if (U < 0.0f || V < 0.0f || W < 0.0f)
    return nullopt;
#else
  if ((U < 0.0f || V < 0.0f || W < 0.0f) && (U > 0.0f || V > 0.0f || W > 0.0f))
    return nullopt;
#endif

  // calculate determinant
  float det = U + V + W;
  if (det == 0.0f)
    return nullopt;
  // Calculate scaled z−coordinates of vertices
  // and use them to calculate the hit distance.
  const float Az = Sz * A[kz];
  const float Bz = Sz * B[kz];
  const float Cz = Sz * C[kz];
  const float T = U * Az + V * Bz + W * Cz;

  // TODO(nicholasbishop): missing from the paper
  vec4 hit;
  hit.w = T;

#ifdef BACKFACE_CULLING
  if (T < 0.0f || T > hit.t * det)
    return nullopt;
#else
  int det_sign = sign_mask(det);
  if ((xorf(T, det_sign) < 0.0f) ||
      xorf(T, det_sign) > hit.t * xorf(det, det_sign))
    return nullopt;
#endif

  // normalize U, V, W, and T
  const float rcpDet = 1.0f / det;
  hit.x = U * rcpDet;
  hit.y = V * rcpDet;
  hit.z = W * rcpDet;
  hit.w = T * rcpDet;
  return hit;
}

}  // namespace mgrid
