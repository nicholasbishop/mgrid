#include "vao.hh"

#include "errors.hh"

namespace mgrid {

Vao::Vao() {
  glGenVertexArrays(1, &handle_);
  if (!handle_) {
    throw GLError("glGenVertexArrays failed");
  }
}

Vao::Vao(Vao&& other) {
  std::swap(handle_, other.handle_);
}

Vao::~Vao() {
  glDeleteVertexArrays(1, &handle_);
}

Vao& Vao::operator=(Vao&& other) {
  std::swap(handle_, other.handle_);
  return *this;
}

void Vao::bind() {
  glBindVertexArray(handle_);
}

GLuint Vao::create_buffer() {
  GLuint buf = 0;
  glGenBuffers(1, &buf);
  if (!buf) {
    throw GLError("glGenBuffers failed");
  }
  return buf;
}

}
