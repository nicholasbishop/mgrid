#ifndef SRC_COMMON_HH_
#define SRC_COMMON_HH_

#include <cstdio>
#include <cstdlib>

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

#endif  // SRC_COMMON_HH_
