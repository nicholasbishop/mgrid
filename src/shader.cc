#include "shader.hh"

#include <iostream>
#include <string>

namespace mgrid {

GLuint compile_shader(const GLenum kind, const std::string& contents) {
  const char* c_str = contents.c_str();
  const int count = 1;

  const auto shader = glCreateShader(kind);
  glShaderSource(shader, count, &c_str, NULL);
  glCompileShader(shader);

  GLint log_length = 0;
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
  std::string log;
  log.resize(log_length);
  glGetShaderInfoLog(shader, log_length, &log_length, &log[0]);
  log.resize(log_length);

  std::cout << log << std::endl;

  return shader;
}

}
