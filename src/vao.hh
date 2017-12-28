#ifndef SRC_VAO_HH_
#define SRC_VAO_HH_

#include <epoxy/gl.h>

namespace mgrid {

class Vao {
 public:
  Vao();
  Vao(Vao&&);
  Vao(Vao&) = delete;

  ~Vao();

  Vao& operator=(Vao&&);

  void bind();

 private:
  GLuint handle_ = 0;
};
}

#endif  // SRC_VAO_HH_
