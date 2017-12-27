#include "texture.hh"

#include <utility>

#include "errors.hh"

namespace mgrid {

Texture::Texture(GLenum target) : target_(target), handle_(0) {
  glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
  if (!handle_) {
    throw GLError("glCreateTextures failed");
  }
}

Texture::Texture(Texture &&other) {
  std::swap(target_, other.target_);
  std::swap(handle_, other.handle_);
}

Texture::~Texture() {
  glDeleteTextures(1, &handle_);
}

Texture& Texture::operator=(Texture &&other) {
  std::swap(target_, other.target_);
  std::swap(handle_, other.handle_);
  return *this;
}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, handle_);
}

}
