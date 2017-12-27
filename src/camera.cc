#include "camera.hh"

namespace mgrid {

Camera::Camera() {
  update();
}

float3 Camera::position() const {
  return position_;
}

float4x4 Camera::view_matrix() const {
  return view_matrix_;
}

float4x4 Camera::projection_matrix() const {
  return projection_matrix_;
}

float4x4 Camera::view_projection_matrix() const {
  return view_projection_matrix_;
}

Angle Camera::height_angle() const {
  return height_angle_;
}

Angle Camera::around_angle() const {
  return around_angle_;
}

float Camera::aspect_ratio() const {
  if (height_ == 0) {
    return 1.0f;
  } else {
    return static_cast<float>(width_) / static_cast<float>(height_);
  }
}

void Camera::set_height_angle(const Angle angle) {
  height_angle_ = angle;
  update();
}

void Camera::set_around_angle(const Angle angle) {
  around_angle_ = angle;
  update();
}

void Camera::set_size(const int width, const int height) {
  width_ = width;
  height_ = height;
  update();
}

void Camera::update() {
  // TODO
  view_matrix_ = linalg::identity;
  position_[0] = 0;
  position_[1] = distance_;
  position_[2] = 0;

  view_matrix_.w.y = distance_;

  const auto fovy = Angle::from_degrees(70);
  const float near = 0.01f;
  const float far = 1000.0f;
  projection_matrix_ = linalg::perspective_matrix(
      fovy.in_radians(), aspect_ratio(), near, far);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
  view_projection_matrix_ = linalg::identity;
}

}
