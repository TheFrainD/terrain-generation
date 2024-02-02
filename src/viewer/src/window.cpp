#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <string>
#include <utility>

namespace {
void GlfwErrorCallback(int error_code, const char *message) {
  throw viewer::WindowException {"GLFW occured! Code: " + std::to_string(error_code) +
                                 ". Message: " + std::string(message)};
}
}  // namespace

namespace viewer {
Window::Window(const std::uint32_t width, const std::uint32_t height, const std::string &title)
    : data_ {width, height, title} {
  glfwSetErrorCallback(GlfwErrorCallback);

  // Initialize GLFW
  if (!glfwInit()) {
    throw WindowException {"GLFW failed to initialize"};
  }
  spdlog::debug("GLFW initialized");

  // Set OpenGL version to 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Need for macOS support
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  // Use OpenGL Core Profile
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  handle_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), nullptr, nullptr);
  if (handle_ == nullptr) {
    glfwTerminate();
    throw WindowException {"Failed to create a window"};
  }
  spdlog::debug("Window created");

  // Make window use our own data struct so that we can access it in callbacks
  glfwSetWindowUserPointer(handle_, &data_);

  glfwSetFramebufferSizeCallback(handle_, [](GLFWwindow *window, int width, int height) {
    auto *const data = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    data->width      = width;
    data->height     = height;
    glViewport(0, 0, width, height);
  });

  glfwMakeContextCurrent(handle_);

  // Enable Vsync
  glfwSwapInterval(1);

  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    glfwDestroyWindow(handle_);
    glfwTerminate();
    throw WindowException {"GLEW failed to initialize"};
  }
  spdlog::debug("GLEW initialized");

  spdlog::debug("Vendor: {}", reinterpret_cast<const char *>(glGetString(GL_VENDOR)));
  spdlog::debug("Renderer: {}", reinterpret_cast<const char *>(glGetString(GL_RENDERER)));
  spdlog::debug("OpenGL version: {}", reinterpret_cast<const char *>(glGetString(GL_VERSION)));
  spdlog::debug("GLSL version: {}", reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

Window::~Window() {
  if (handle_ != nullptr) {
    glfwDestroyWindow(handle_);
  }
  glfwTerminate();
}

void Window::SwapBuffer() const {
  if (handle_ == nullptr) {
    throw WindowException {"Window handle is nullptr"};
  }
  glfwSwapBuffers(handle_);
}

bool Window::ShouldClose() const {
  if (handle_ == nullptr) {
    throw WindowException {"Window handle is nullptr"};
  }
  return glfwWindowShouldClose(handle_);
}

void Window::PollEvents() noexcept { glfwPollEvents(); }

WindowException::WindowException(std::string message) : message_(std::move(message)) {}

const char *WindowException::what() const noexcept { return message_.c_str(); }
}  // namespace viewer
