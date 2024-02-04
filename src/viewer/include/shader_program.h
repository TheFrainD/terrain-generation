#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

#include "shader.h"

namespace viewer {
class ShaderProgramLinkException : public std::runtime_error {
public:
  explicit ShaderProgramLinkException(const std::string &log) : std::runtime_error(log) {}
};

class ShaderProgram {
public:
  ShaderProgram();
  ~ShaderProgram();

  ShaderProgram(const ShaderProgram &)            = delete;
  ShaderProgram &operator=(const ShaderProgram &) = delete;

  ShaderProgram(ShaderProgram &&other);
  ShaderProgram &operator=(ShaderProgram &&other) noexcept;

  void Attach(const Shader &shader) const;
  void Link() const;
  void Use() const;

private:
  std::uint32_t id_;

  void Release() noexcept;
  std::string GetInfoLog() const;
};
}  // namespace viewer
