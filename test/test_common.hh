#ifndef TEST_TEST_COMMON_HH_
#define TEST_TEST_COMMON_HH_

#include "glm/gtc/epsilon.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/io.hpp"

constexpr auto epsilon = FLT_EPSILON * 10;

inline void assert_equalish(const vec3& a, const vec3& b) {
  INFO("a: " << a);
  INFO("b: " << b);
  REQUIRE(epsilonEqual(a, b, epsilon) == glm::bvec3{true});
}

inline void assert_equalish(const vec4& a, const vec4& b) {
  INFO("a: " << a);
  INFO("b: " << b);
  REQUIRE(epsilonEqual(a, b, epsilon) == glm::bvec4{true});
}

#endif  // TEST_TEST_COMMON_HH_
