#include "vao.hh"

#include <cassert>

#include "errors.hh"

namespace mgrid {

Vbo::Vbo(const GLenum kind) : kind_(kind) {
  // TODO(nicholasbishop): allow element buffer
  assert(kind == GL_ARRAY_BUFFER);

  glGenBuffers(1, &handle_);
  if (!handle_) {
    throw GLError("glGenBuffers failed");
  }
}

Vbo::Vbo(Vbo&& other) {
  std::swap(handle_, other.handle_);
  std::swap(kind_, other.kind_);
}

Vbo::~Vbo() {
  glDeleteBuffers(1, &handle_);
}

Vbo& Vbo::operator=(Vbo&& other) {
  std::swap(handle_, other.handle_);
  std::swap(kind_, other.kind_);
  return *this;
}

void Vbo::bind() {
  glBindBuffer(kind_, handle_);
}

void Vbo::set_data(const void* data,
                   const std::size_t length,
                   const GLenum hint) {
  bind();
  // TODO(nicholasbishop): allow other hints
  assert(hint == GL_STATIC_DRAW);
  glBufferData(kind_, length, data, hint);
}

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

void Vao::set_attribute_data(const GLint attrib,
                             const int components_per_vertex,
                             const GLenum type,
                             const void* data) {
  bind();
  glEnableVertexAttribArray(attrib);

  const int stride = 0;
  const bool normalized = false;
  glVertexAttribPointer(attrib, components_per_vertex, type, normalized, stride,
                        data);
}
}
