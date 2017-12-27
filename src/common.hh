#ifndef SRC_COMMON_HH_
#define SRC_COMMON_HH_

#include <cstdio>
#include <cstdlib>
#include <string>

#include <experimental/optional>

#include "glm/fwd.hpp"

#ifdef __GNUC__
#define UNUSED(x) UNUSED_##x __attribute__((__unused__))
#else
#define UNUSED(x) UNUSED_##x
#endif

#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

using glm::vec2;
using glm::vec3;
using glm::mat4;

using std::experimental::optional;
using std::experimental::nullopt;

#endif // SRC_COMMON_HH_
