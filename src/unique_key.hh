#include <cstdint>
#include <functional>
#include <ostream>
#include <string>

#include "external/json.hpp"

template <typename T>
class UniqueKey {
 public:
  UniqueKey() {}

  std::size_t hash() const { return std::hash<uint64_t>{}(value_); }

  bool operator==(const UniqueKey<T>& other) const {
    return value_ == other.value_;
  }

  operator std::string() const { return std::to_string(value_); }

  bool operator<(const std::string& str) const {
    return std::to_string(value_) < str;
  }

  void advance() { value_++; }

  bool valid() const { return value_ != invalid_value_; }

 private:
  static constexpr uint64_t invalid_value_ = 0;

  explicit UniqueKey(uint64_t value) : value_{value} {}

  uint64_t value_ = invalid_value_;
};

template <typename T>
void to_json(nlohmann::json& j, const UniqueKey<T>& key) {
  j = static_cast<std::string>(key);
}

template <typename T>
std::ostream& operator<<(std::ostream& stream, const UniqueKey<T>& key) {
  stream << static_cast<std::string>(key);
  return stream;
}

namespace std {
template <typename T>
struct hash<UniqueKey<T>> {
  typedef UniqueKey<T> argument_type;
  typedef std::size_t result_type;
  result_type operator()(argument_type const& key) const noexcept {
    return key.hash();
  }
};
}
