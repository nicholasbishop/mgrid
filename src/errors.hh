#ifndef SRC_ERROR_HH_
#define SRC_ERROR_HH_

#include <stdexcept>

namespace mgrid {

class GLError : public std::runtime_error {
 public:
  GLError(const std::string& what);
};

void check_gl_error(const std::string& where = "");

}  // namespace mgrid

#endif  // SRC_ERROR_HH_
