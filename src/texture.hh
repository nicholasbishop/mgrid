#ifndef SRC_TEXTURE_HH_
#define SRC_TEXTURE_HH_

#include <epoxy/gl.h>

namespace mgrid {

class Texture {
 public:
  explicit Texture(GLenum target);
  Texture(Texture &&other);
  Texture(Texture &) = delete;
  ~Texture();

  Texture& operator=(Texture &&other);

  void bind();

 private:
  GLenum target_;
  GLuint handle_;
};

}

#endif  // SRC_TEXTURE_HH_
