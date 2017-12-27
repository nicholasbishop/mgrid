#ifndef SRC_COMMON_HH_
#define SRC_COMMON_HH_

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#endif  // SRC_COMMON_HH_
