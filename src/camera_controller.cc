#include "camera_controller.hh"

#include <cassert>

#include "camera.hh"

namespace mgrid {

CameraController::CameraController(Camera& camera) : camera_(camera) {}

CameraController::~CameraController() {}

void CameraController::start_rotate(const vec2& pos) {
  orig_around_angle_ = camera_.around_angle();
  orig_height_angle_ = camera_.height_angle();
  orig_pos_ = pos;
  in_rotate_ = true;
}

void CameraController::set_rotate(const vec2& pos) {
  assert(in_rotate_);

  const auto delta = pos - orig_pos_;

  const auto delta_around{Angle::from_degrees(delta.x / 2.0f)};
  const auto delta_height{Angle::from_degrees(delta.y / 2.0f)};

  camera_.set_around_angle(orig_around_angle_ - delta_around);
  camera_.set_height_angle(orig_height_angle_ + delta_height);
}

void CameraController::end_rotate() {
  in_rotate_ = false;
}
}
