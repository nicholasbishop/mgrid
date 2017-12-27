#ifndef SRC_RAY_HH_
#define SRC_RAY_HH_

#include "glm/vec3.hpp"

#include "common.hh"

namespace mgrid {

class Ray3 {
 public:
  const vec3 origin;
  const vec3 direction;
};

}  // namespace mgrid

#endif  // SRC_RAY_HH_
