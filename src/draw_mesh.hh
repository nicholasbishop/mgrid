#ifndef SRC_DRAW_MESH_HH_
#define SRC_DRAW_MESH_HH_

#include <vector>

#include "shader.hh"
#include "texture.hh"
#include "vao.hh"

namespace mgrid {

class DrawMesh {
 public:
  Vao& vao() { return vao_; }

  ShaderProgram& program() { return program_; }

  Vbo& add_array_buffer();

  Texture& add_texture();

 private:
  Vao vao_;
  ShaderProgram program_;
  std::vector<Vbo> vbos_;
  std::vector<Texture> textures_;
};

}  // namespace mgrid

#endif  // SRC_DRAW_MESH_HH_
