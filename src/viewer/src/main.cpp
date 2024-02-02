#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "window.h"

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::trace);
  try {
    auto window {std::make_unique<viewer::Window>(640, 480, "Viewer")};
    while (!window->ShouldClose()) {
      window->SwapBuffer();
      viewer::Window::PollEvents();
    }
  } catch (const viewer::WindowException &ex) {
    spdlog::error("Window error: {}", ex.what());
    return -1;
  } catch (...) {
    spdlog::critical("Unknown error");
    return -1;
  }

  return 1;
}
