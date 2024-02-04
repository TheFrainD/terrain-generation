#include "shader_program.h"

#include <GL/glew.h>

#include <utility>

namespace viewer {
ShaderProgram::ShaderProgram() : id_(glCreateProgram()) {}

ShaderProgram::~ShaderProgram() { Release(); }

ShaderProgram::ShaderProgram(ShaderProgram &&other) : id_(other.id_) { other.id_ = 0; }

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&other) noexcept {
  if (this != &other) {
    Release();
    std::swap(id_, other.id_);
  }
  return *this;
}

void ShaderProgram::Attach(const Shader &shader) const { glAttachShader(id_, shader.GetId()); }

void ShaderProgram::Link() const {
  glLinkProgram(id_);

  GLint link_status;
  glGetProgramiv(id_, GL_LINK_STATUS, &link_status);
  if (!link_status) {
    throw ShaderProgramLinkException {GetInfoLog()};
  }
}

void ShaderProgram::Use() const { glUseProgram(id_); }

void ShaderProgram::Release() noexcept {
  if (id_ != 0) {
    glDeleteProgram(id_);
    id_ = 0;
  }
}

std::string ShaderProgram::GetInfoLog() const {
  GLint length;
  glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &length);

  if (length > 0) {
    std::string info_log(length, 0);
    glGetProgramInfoLog(id_, length, &length, info_log.data());
    return info_log;
  }

  return "";
}
}  // namespace viewer
