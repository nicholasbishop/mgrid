#ifndef SRC_SHADER_HH_
#define SRC_SHADER_HH_

#include <string>

#include <epoxy/gl.h>

namespace mgrid {

GLuint compile_shader(GLenum kind, const std::string& contents);

}

#endif  // SRC_SHADER_HH_
