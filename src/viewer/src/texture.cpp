#include "texture.h"

#include <GL/glew.h>

#include <utility>

namespace viewer {
Texture::Texture() { glGenTextures(1, &id_); }

Texture::~Texture() { Release(); }

Texture::Texture(Texture &&other) : id_(other.id_) { other.id_ = 0; }

Texture &Texture::operator=(Texture &&other) noexcept {
  if (this != &other) {
    Release();
    std::swap(id_, other.id_);
  }
  return *this;
}

void Texture::Bind(const std::uint32_t slot) const noexcept {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id_);
}

void Texture::SetData(const std::vector<std::uint8_t> &data, const std::uint32_t width,
                      const std::uint32_t height) const noexcept {
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetImage(const Image &image) const noexcept {
  auto format {image.GetFormat() == Image::Format::kRgba ? GL_RGBA : GL_RGB};
  glTexImage2D(GL_TEXTURE_2D, 0, format, image.GetWidth(), image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               image.GetData());
}

void Texture::SetWrapping(const Wrapping s, const Wrapping t) const noexcept {
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(s));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(t));
}

void Texture::SetFilters(const Filter min, const Filter mag) const noexcept {
  glBindTexture(GL_TEXTURE_2D, id_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(min));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mag));
}

void Texture::GenerateMipmaps() const noexcept {
  glBindTexture(GL_TEXTURE_2D, id_);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Release() noexcept {
  if (id_ != 0) {
    glDeleteTextures(1, &id_);
    id_ = 0;
  }
}
}  // namespace viewer
