#include "errors.hh"

#include <epoxy/gl.h>

namespace mgrid {

GLError::GLError(const std::string& what) : runtime_error(what) {}

void check_gl_error(const std::string& where) {
  const auto err = glGetError();
  if (err == GL_NO_ERROR) {
    return;
  }

  std::string str = "unknown";
  switch (err) {
    case GL_INVALID_ENUM:
      str = "invalid enum";
      break;

    case GL_INVALID_VALUE:
      str = "invalid value";
      break;

    case GL_INVALID_OPERATION:
      str = "invalid operation";
      break;
  }

  if (!where.empty()) {
    str = where + ": " + str;
  }
  throw GLError("glGetError: " + str);
}

}
