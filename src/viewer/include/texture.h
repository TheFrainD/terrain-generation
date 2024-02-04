#pragma once

#include <cstdint>
#include <vector>

#include "image.h"

namespace viewer {
class Texture {
public:
  enum class Wrapping : std::uint32_t {
    kClampToEdge = 0x812F,
    kRepeat      = 0x2901,
  };

  enum class Filter : std::uint32_t {
    kNearest              = 0x2600,
    kLinear               = 0x2601,
    kNearestMipmapNearest = 0x2700,
    kLinearMipmapNearest  = 0x2701,
    kNearestMipmapLinear  = 0x2702,
    kLinearMipmapLinear   = 0x2703,
  };

  Texture();
  ~Texture();

  Texture(const Texture &)            = delete;
  Texture &operator=(const Texture &) = delete;

  Texture(Texture &&other);
  Texture &operator=(Texture &&other) noexcept;

  void Bind(std::uint32_t slot = 0) const noexcept;
  void SetData(const std::vector<std::uint8_t> &data, std::uint32_t width, std::uint32_t height) const noexcept;
  void SetImage(const Image &image) const noexcept;
  void SetWrapping(Wrapping s, Wrapping t) const noexcept;
  void SetFilters(Filter min, Filter mag) const noexcept;
  void GenerateMipmaps() const noexcept;

private:
  std::uint32_t id_;

  void Release() noexcept;
};
}  // namespace viewer
