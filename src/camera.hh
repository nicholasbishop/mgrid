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
  
  float4x4 view_matrix() const;
  float4x4 projection_matrix() const;
  float4x4 view_projection_matrix() const;

  float3 target() const;
  Angle height_angle() const;
  Angle around_angle() const;
  float distance() const;
  float aspect_ratio() const;

  void set_target(float3);
  void set_height_angle(Angle);
  void set_around_angle(Angle);
  void set_distance(float);

  void set_size(int width, int height);

 private:
  void update();

  float3 target_;
  Angle height_angle_ = Angle::from_degrees(35);
  Angle around_angle_ = Angle::from_degrees(35);
  float distance_ = 10;

  int width_ = 1;
  int height_ = 1;

  float3 position_;
  float4x4 view_matrix_;
  float4x4 projection_matrix_;
  float4x4 view_projection_matrix_;
};

}

#endif  // SRC_CAMERA_HH_
