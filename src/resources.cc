#include "cmrc/cmrc.hpp"

#include "resources.hh"

namespace mgrid {

std::string read_resource_string(const std::string& path) {
  auto r = cmrc::open(path);
  if (!r.begin()) {
    throw std::runtime_error("invalid resource: " + path);
  }
  return {r.begin(), r.end()};
}

}  // namespace mgrid
