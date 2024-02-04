#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace viewer {
class ShaderCompileException : public std::runtime_error {
public:
  explicit ShaderCompileException(const std::string &log) : std::runtime_error(log) {}
};

class Shader {
public:
  enum class Type : std::uint32_t { kVertex = 0x8B31, kFragment = 0x8B30, kGeometry = 0x8DD };

  Shader(Type type, const std::vector<std::uint8_t> binary);
  ~Shader();

  Shader(const Shader &)            = delete;
  Shader &operator=(const Shader &) = delete;

  Shader(Shader &&) noexcept;
  Shader &operator=(Shader &&) noexcept;

  std::uint32_t GetId() const noexcept;

private:
  std::uint32_t id_;

  void Release() noexcept;
  std::string GetInfoLog();
};
}  // namespace viewer
