#ifndef SRC_CAMERA_HH_
#define SRC_CAMERA_HH_

#include "angle.hh"
#include "common.hh"
#include "linalg.h"

namespace mgrid {

class Camera {
 public:
  using float3 = linalg::aliases::float3;
  using float4x4 = linalg::aliases::float4x4;

  Camera();

  float3 position() const;

  float3 target() const;
  Angle height_angle() const;
  Angle around_angle() const;
  float distance() const;

  void set_target(float3);
  void set_height_angle(Angle);
  void set_around_angle(Angle);
  void set_distance(float);

  float4x4 view_matrix() const;

 private:
  void update();

  float3 target_;
  Angle height_angle_ = Angle::from_degrees(35);
  Angle around_angle_ = Angle::from_degrees(35);
  float distance_ = 10;

  float3 position_;
  float4x4 view_matrix_;
};

}

#endif  // SRC_CAMERA_HH_
