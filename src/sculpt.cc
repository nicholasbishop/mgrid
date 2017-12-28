#include "sculpt.hh"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

#include "grid.hh"

namespace mgrid {

Sculpt::Sculpt(Grid& grid) : grid_(grid) {}

Sculpt::~Sculpt() {}

void Sculpt::start() {
  in_drag_ = true;
}

void Sculpt::move(const vec3& center) {
  assert(in_drag_);

  const float radius_squared = radius_ * radius_;
  grid_.points_iter([&](vec3& point) {
      const float dsq = glm::distance2(center, point);
      if (dsq < radius_squared) {
        const float dist = sqrtf(dsq);
        point.z += (radius_ - dist) * strength_;
      }
  });
}

void Sculpt::stop() {
  in_drag_ = false;
}

}  // mgrid
