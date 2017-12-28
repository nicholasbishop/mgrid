#include "draw_mesh.hh"

#include "errors.hh"

namespace mgrid {

Vbo* DrawMesh::add_array_buffer() {
  vbos_.emplace_back(GL_ARRAY_BUFFER);
  return &vbos_.back();
}

Texture* DrawMesh::add_texture() {
  textures_.emplace_back(GL_TEXTURE_2D);
  return &textures_.back();
}

void DrawMesh::draw(const GLenum mode, const int num_vertices) {
  assert(mode == GL_PATCHES || mode == GL_LINES);
  vao_.bind();
  program_.bind();
  check_gl_error("a");
  glDrawArrays(mode, 0, num_vertices);
  check_gl_error("b");
}

}  // namespace mgrid
