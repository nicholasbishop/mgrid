#ifndef SRC_CAMERA_HH_
#define SRC_CAMERA_HH_

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include "angle.hh"
#include "common.hh"
#include "ray.hh"

namespace mgrid {

class Camera {
 public:
  Camera();

  vec3 position() const;

  mat4 view_matrix() const;
  mat4 projection_matrix() const;
  mat4 view_projection_matrix() const;

  vec3 target() const;
  Angle height_angle() const;
  Angle around_angle() const;
  float distance() const;

  vec3 unproject(const vec3& win) const;
  Ray3 ray(const vec2& win) const;

  void set_target(vec3);
  void set_height_angle(Angle);
  void set_around_angle(Angle);
  void set_distance(float);

  void set_size(const ivec2& size);

 private:
  void update();

  vec3 target_;
  Angle height_angle_ = Angle::from_degrees(80);
  Angle around_angle_ = Angle::from_degrees(80);
  float distance_ = 2;

  ivec2 size_ = {1, 1};

  vec3 position_;
  mat4 view_matrix_;
  mat4 projection_matrix_;
  mat4 view_projection_matrix_;
};
}

#endif  // SRC_CAMERA_HH_
