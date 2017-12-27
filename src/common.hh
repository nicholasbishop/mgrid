#ifndef SRC_COMMON_HH_
#define SRC_COMMON_HH_

#include <cstdio>
#include <cstdlib>
#include <string>

#include "glm/fwd.hpp"

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

using glm::vec3;
using glm::mat4;

#endif  // SRC_COMMON_HH_
