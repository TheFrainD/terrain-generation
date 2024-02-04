#include <GL/glew.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#include <fstream>
#include <memory>
#include <vector>

#include "image.h"
#include "shader.h"
#include "shader_program.h"
#include "texture.h"
#include "vertex_array.h"
#include "vertex_buffer.h"
#include "window.h"

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::trace);

  try {
    auto window {std::make_unique<viewer::Window>(640, 480, "Viewer")};

    float vertices[] = {
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    std::ifstream vertex_shader_file("shaders/basic.vert.spv", std::ios::binary);
    std::ifstream fragment_shader_file("shaders/basic.frag.spv", std::ios::binary);

    std::vector<std::uint8_t> vertex_shader_buffer((std::istreambuf_iterator<char>(vertex_shader_file)),
                                                   (std::istreambuf_iterator<char>()));
    std::vector<std::uint8_t> fragment_shader_buffer((std::istreambuf_iterator<char>(fragment_shader_file)),
                                                     (std::istreambuf_iterator<char>()));

    viewer::ShaderProgram program;

    {
      viewer::Shader vertex_shader {viewer::Shader::Type::kVertex, vertex_shader_buffer};
      viewer::Shader fragment_shader {viewer::Shader::Type::kFragment, fragment_shader_buffer};

      program.Attach(vertex_shader);
      program.Attach(fragment_shader);
      program.Link();
    }

    viewer::VertexArray vao;

    viewer::VertexBuffer vbo {vertices, sizeof(vertices)};
    viewer::VertexBuffer ebo {indices, sizeof(indices)};

    vao.BindAttribute(vbo, 0, 3, viewer::VertexArray::DataType::kFloat, 5 * sizeof(float), 0);
    vao.BindAttribute(vbo, 1, 2, viewer::VertexArray::DataType::kFloat, 5 * sizeof(float), 3 * sizeof(float));

    viewer::Texture texture;
    texture.SetWrapping(viewer::Texture::Wrapping::kRepeat, viewer::Texture::Wrapping::kRepeat);
    texture.SetFilters(viewer::Texture::Filter::kNearest, viewer::Texture::Filter::kNearest);

    {
      viewer::Image wall {"textures/wall.jpg"};
      texture.SetImage(wall);
    }

    while (!window->ShouldClose()) {
      glClearColor(0.2, 0.3, 0.3, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      texture.Bind();
      program.Use();
      vao.Bind();
      ebo.Bind(viewer::VertexBuffer::Target::kElementArrayBuffer);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      vao.Unbind();
      ebo.Unbind(viewer::VertexBuffer::Target::kElementArrayBuffer);

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
