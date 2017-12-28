#include "camera.hh"

#include "glm/gtc/matrix_transform.hpp"

#include <iostream>

namespace mgrid {

Camera::Camera() {
  update();
}

vec3 Camera::position() const {
  return position_;
}

mat4 Camera::view_matrix() const {
  return view_matrix_;
}

mat4 Camera::projection_matrix() const {
  return projection_matrix_;
}

mat4 Camera::view_projection_matrix() const {
  return view_projection_matrix_;
}

Angle Camera::height_angle() const {
  return height_angle_;
}

Angle Camera::around_angle() const {
  return around_angle_;
}

vec3 Camera::unproject(const vec3& win) const {
  vec4 viewport{0, 0, size_.x, size_.y};
  return glm::unProject(win, view_matrix_, projection_matrix_, viewport);
}

Ray3 Camera::ray(const vec2& win) const {
  vec3 win_origin{win.x, win.y, -1};
  vec3 win_target{win.x, win.y, 1};
  const auto origin = unproject(win_origin);
  const auto target = unproject(win_target);
  return Ray3{origin, glm::normalize(target - origin)};
}

void Camera::set_height_angle(const Angle angle) {
  height_angle_ = angle;
  update();
}

void Camera::set_around_angle(const Angle angle) {
  around_angle_ = angle;
  update();
}

void Camera::set_size(const ivec2& size) {
  size_ = size;
  update();
}

void Camera::set_distance(const float distance) {
  distance_ = distance;
  update();
}

void Camera::update() {
  // TODO
  position_[0] = sin(around_angle_.in_radians()) * distance_;
  position_[2] = cos(around_angle_.in_radians()) * distance_;

  position_[1] = sin(height_angle_.in_radians()) * distance_;

  view_matrix_ = glm::lookAt(position_, target_, vec3(0, 1, 0));

  const auto fovy = Angle::from_degrees(70);
  const float near = 0.1f;
  const float far = 100.0f;
  projection_matrix_ = glm::perspectiveFov<float>(fovy.in_radians(), size_.x,
                                                  size_.y, near, far);
  view_projection_matrix_ = projection_matrix_ * view_matrix_;
}
}
