#include "camera.hh"

namespace mgrid {

Camera::Camera() {
  update();
}

Camera::float3 Camera::position() const {
  return position_;
}

Camera::float4x4 Camera::view_matrix() const {
  return view_matrix_;
}

Angle Camera::height_angle() const {
  return height_angle_;
}

Angle Camera::around_angle() const {
  return around_angle_;
}

void Camera::set_height_angle(const Angle angle) {
  height_angle_ = angle;
  update();
}

void Camera::set_around_angle(const Angle angle) {
  around_angle_ = angle;
  update();
}

void Camera::update() {
  // TODO
  view_matrix_ = linalg::identity;
  position_[0] = 0;
  position_[1] = distance_;
  position_[2] = 0;
}

}
