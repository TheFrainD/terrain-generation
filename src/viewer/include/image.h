#pragma once

#include <cstdint>
#include <stdexcept>

namespace viewer {
class ImageLoadingException : public std::runtime_error {
public:
  explicit ImageLoadingException(const std::string &path)
      : std::runtime_error("Failed to load image at apath \"" + path + "\"") {}
};

class Image {
public:
  enum class Format : std::int32_t { kRgb = 3, kRgba = 4 };

  explicit Image(const std::string &path);
  ~Image();

  Image(const Image &)            = delete;
  Image &operator=(const Image &) = delete;

  Image(Image &&other);
  Image &operator=(Image &&other) noexcept;

  std::int32_t GetWidth() const noexcept;
  std::int32_t GetHeight() const noexcept;
  Format GetFormat() const noexcept;
  std::uint8_t *GetData() const noexcept;

private:
  std::int32_t width_;
  std::int32_t height_;
  Format format_;
  std::uint8_t *data_;

  void Release() noexcept;
};
}  // namespace viewer
