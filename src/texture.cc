#include "texture.hh"

#include <utility>

#include "errors.hh"

namespace mgrid {

Texture::Texture(GLenum target) : target_(target), handle_(0) {
  glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
  if (!handle_) {
    throw GLError("glCreateTextures failed");
  }

  // Set some sensible defaults
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(Texture&& other) {
  std::swap(target_, other.target_);
  std::swap(handle_, other.handle_);
}

Texture::~Texture() {
  glDeleteTextures(1, &handle_);
}

Texture& Texture::operator=(Texture&& other) {
  std::swap(target_, other.target_);
  std::swap(handle_, other.handle_);
  return *this;
}

void Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, handle_);
}
}
