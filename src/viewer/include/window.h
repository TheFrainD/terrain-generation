#pragma once

#include <cstdint>
#include <exception>
#include <string>

// GLFWWindow forward declaration
struct GLFWwindow;

namespace viewer {
struct WindowData {
  std::uint32_t width;
  std::uint32_t height;
  std::string title;
};

class Window {
public:
  Window(std::uint32_t width, std::uint32_t height, const std::string &title);
  ~Window();

  Window(const Window &)            = delete;
  Window(Window &&)                 = delete;
  Window &operator=(const Window &) = delete;
  Window &operator=(Window &&)      = delete;

  void SwapBuffer() const;
  bool ShouldClose() const;

  static void PollEvents() noexcept;

private:
  GLFWwindow *handle_ {nullptr};
  WindowData data_;
};

class WindowException : public std::exception {
public:
  explicit WindowException(std::string message);
  const char *what() const noexcept override;

private:
  std::string message_;
};
}  // namespace viewer
