#include "draw_mesh.hh"

namespace mgrid {

Vbo& DrawMesh::add_array_buffer() {
  vbos_.emplace_back(GL_ARRAY_BUFFER);
  return vbos_.back();
}

Texture& DrawMesh::add_texture() {
  textures_.emplace_back(GL_TEXTURE_2D);
  return textures_.back();
}

}  // namespace mgrid
