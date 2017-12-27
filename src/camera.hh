#ifndef SRC_CAMERA_HH_
#define SRC_CAMERA_HH_

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include "angle.hh"
#include "common.hh"

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
  float aspect_ratio() const;

  void set_target(vec3);
  void set_height_angle(Angle);
  void set_around_angle(Angle);
  void set_distance(float);

  void set_size(int width, int height);

private:
  void update();

  vec3 target_;
  Angle height_angle_ = Angle::from_degrees(80);
  Angle around_angle_ = Angle::from_degrees(80);
  float distance_ = 4;

  int width_ = 1;
  int height_ = 1;

  vec3 position_;
  mat4 view_matrix_;
  mat4 projection_matrix_;
  mat4 view_projection_matrix_;
};
}

#endif // SRC_CAMERA_HH_
