#include "vertex_buffer.h"

#include <GL/glew.h>

#include <utility>

namespace viewer {
VertexBuffer::VertexBuffer(const void *data, const std::size_t size, VertexBuffer::Usage usage) : size_(size) {
  glGenBuffers(1, &id_);
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  glBufferData(GL_ARRAY_BUFFER, size_, data, static_cast<GLenum>(usage));
}

VertexBuffer::VertexBuffer(std::size_t size) : VertexBuffer(nullptr, size, Usage::kDynamicDraw) {}

VertexBuffer::~VertexBuffer() { Release(); }

VertexBuffer::VertexBuffer(VertexBuffer &&other) noexcept : id_(other.id_), size_(other.size_) {
  other.id_   = 0;
  other.size_ = 0;
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept {
  if (this != &other) {
    Release();
    std::swap(id_, other.id_);
    std::swap(size_, other.size_);
  }
  return *this;
}

void VertexBuffer::Bind(const Target target) const noexcept { glBindBuffer(static_cast<GLenum>(target), id_); }

void VertexBuffer::Unbind(const Target target) noexcept { glBindBuffer(static_cast<GLenum>(target), 0); }

void VertexBuffer::SetData(const void *data, const std::size_t size) noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  size_ = size;
  glBufferData(GL_ARRAY_BUFFER, size_, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::SubData(const void *data, std::size_t offset, const std::size_t size) noexcept {
  glBindBuffer(GL_ARRAY_BUFFER, id_);
  size_ = size;
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

std::uint32_t VertexBuffer::GetId() const noexcept { return id_; }

void VertexBuffer::Release() noexcept {
  if (id_ != 0) {
    glDeleteBuffers(1, &id_);
    id_ = 0;
  }
}
}  // namespace viewer
