#ifndef SRC_CAMERA_CONTROLLER_HH_
#define SRC_CAMERA_CONTROLLER_HH_

#include "glm/vec2.hpp"

#include "angle.hh"
#include "common.hh"

namespace mgrid {

class Camera;

class CameraController {
 public:
  CameraController(Camera& camera);

  ~CameraController();

  void start_rotate(const vec2& pos);

  void set_rotate(const vec2& pos);

  void end_rotate();

  bool in_rotate() const { return in_rotate_; }

 private:
  Camera& camera_;
  Angle orig_around_angle_;
  Angle orig_height_angle_;
  vec2 orig_pos_;
  bool in_rotate_ = false;
};

}  // namespace mgrid

#endif  // SRC_CAMERA_CONTROLLER_HH_
