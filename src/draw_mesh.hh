#ifndef SRC_DRAW_MESH_HH_
#define SRC_DRAW_MESH_HH_

#include <list>

#include "shader.hh"
#include "texture.hh"
#include "vao.hh"

namespace mgrid {

class DrawMesh {
 public:
  Vao& vao() { return vao_; }

  ShaderProgram& program() { return program_; }

  Vbo* add_array_buffer();

  Texture* add_texture();

  void draw(const GLenum mode, const int num_vertices);

 private:
  Vao vao_;
  ShaderProgram program_;
  std::list<Vbo> vbos_;
  std::list<Texture> textures_;
};

}  // namespace mgrid

#endif  // SRC_DRAW_MESH_HH_
