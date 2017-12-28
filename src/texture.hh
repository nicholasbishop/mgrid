#ifndef SRC_TEXTURE_HH_
#define SRC_TEXTURE_HH_

#include <epoxy/gl.h>

#include "common.hh"

namespace mgrid {

class Texture {
 public:
  explicit Texture(GLenum target);
  Texture(Texture&& other);
  Texture(Texture&) = delete;
  ~Texture();

  Texture& operator=(Texture&& other);

  void bind();

  void set_data(
      GLint internalFormat,
      const ivec2& size,
      GLenum format,
      GLenum type,
      const GLvoid* data);

  GLuint handle() const { return handle_; }

 private:
  GLenum target_;
  GLuint handle_;
};
}

#endif  // SRC_TEXTURE_HH_
