#include "image.h"

#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace viewer {
Image::Image(const std::string &path)
    : data_(stbi_load(path.c_str(), &width_, &height_, reinterpret_cast<int *>(&format_), STBI_rgb_alpha)) {
  if (!data_) {
    throw ImageLoadingException {path};
  }
}

Image::~Image() { Release(); }

Image::Image(Image &&other) : width_(other.width_), height_(other.height_), format_(other.format_), data_(other.data_) {
  other.data_ = nullptr;
}

Image &Image::operator=(Image &&other) noexcept {
  if (this != &other) {
    Release();
    std::swap(width_, other.width_);
    std::swap(height_, other.height_);
    std::swap(format_, other.format_);
    std::swap(data_, other.data_);
  }
  return *this;
}

std::int32_t Image::GetWidth() const noexcept { return width_; }

std::int32_t Image::GetHeight() const noexcept { return height_; }

Image::Format Image::GetFormat() const noexcept { return format_; }

std::uint8_t *Image::GetData() const noexcept { return data_; }

void Image::Release() noexcept {
  if (data_ != nullptr) {
    stbi_image_free(data_);
    data_ = nullptr;
  }
}
}  // namespace viewer
