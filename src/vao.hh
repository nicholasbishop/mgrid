#ifndef SRC_VAO_HH_
#define SRC_VAO_HH_

#include <cstdint>

#include <epoxy/gl.h>

namespace mgrid {

class Vbo {
 public:
  Vbo(GLenum type);
  Vbo(Vbo&&);
  Vbo(Vbo&) = delete;

  ~Vbo();

  Vbo& operator=(Vbo&&);

  void bind();

  void set_data(const void* data, const std::size_t length, const GLenum hint);

 private:
  GLenum kind_{};
  GLuint handle_{};
};

class Vao {
 public:
  Vao();
  Vao(Vao&&);
  Vao(Vao&) = delete;

  ~Vao();

  Vao& operator=(Vao&&);

  void bind();

  void set_attribute_data(const GLint attrib,
                          const int components_per_vertex,
                          const GLenum component_type,
                          const void* data);

 private:
  GLuint handle_{};
};
}

#endif  // SRC_VAO_HH_
