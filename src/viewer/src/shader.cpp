#include "shader.h"

#include <GL/glew.h>

#include <utility>

namespace viewer {
Shader::Shader(const Type type, const std::vector<std::uint8_t> binary)
    : id_(glCreateShader(static_cast<GLenum>(type))) {
  glShaderBinary(1, &id_, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size());
  glSpecializeShader(id_, "main", 0, nullptr, nullptr);

  GLint compile_status;
  glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);

  if (!compile_status) {
    throw ShaderCompileException {GetInfoLog()};
  }
}

Shader::~Shader() { Release(); }

Shader::Shader(Shader &&other) noexcept : id_(other.id_) { other.id_ = 0; }

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    Release();
    std::swap(id_, other.id_);
  }
  return *this;
}

std::uint32_t Shader::GetId() const noexcept { return id_; }

void Shader::Release() noexcept {
  if (id_ != 0) {
    glDeleteShader(id_);
    id_ = 0;
  }
}

std::string Shader::GetInfoLog() {
  GLint length;
  glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &length);

  if (length > 0) {
    std::string info_log(length, 0);
    glGetShaderInfoLog(id_, length, &length, info_log.data());
    return info_log;
  }

  return "";
}
}  // namespace viewer
