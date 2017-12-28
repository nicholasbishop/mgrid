#ifndef TEST_STRINGIFY_HH_
#define TEST_STRINGIFY_HH_

#include <array>
#include <ostream>
#include <string>
#include <vector>

template <typename I>
void write_iterable_to_stream(std::ostream& stream, const I& iterable) {
  stream << "{";
  bool first = true;
  for (const auto& elem : iterable) {
    if (first) {
      first = false;
    } else {
      stream << ", ";
    }
    stream << elem;
  }
  stream << "}";
}

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& stream, const std::array<T, N>& arr) {
  stream << "array";
  write_iterable_to_stream(stream, arr);
  return stream;
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& vec) {
  stream << "vector";
  write_iterable_to_stream(stream, vec);
  return stream;
}

#endif  // TEST_STRINGIFY_HH_
