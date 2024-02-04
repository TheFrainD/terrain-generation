#include <GL/glew.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cstdint>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <memory>
#include <vector>

#include "shader.h"
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

    GLuint program = glCreateProgram();

    {
      viewer::Shader vertex_shader {viewer::Shader::Type::kVertex, vertex_shader_buffer};
      viewer::Shader fragment_shader {viewer::Shader::Type::kFragment, fragment_shader_buffer};

      glAttachShader(program, vertex_shader.GetId());
      glAttachShader(program, fragment_shader.GetId());
      glLinkProgram(program);
      GLint success;
      char info_log[512];
      glGetProgramiv(program, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, info_log);
        spdlog::error("Linking error: {}", info_log);
      }
    }

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, ch;
    unsigned char *data = stbi_load("textures/wall.jpg", &width, &height, &ch, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      spdlog::error("Failed to load texture");
    }
    stbi_image_free(data);

    while (!window->ShouldClose()) {
      glClearColor(0.2, 0.3, 0.3, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glBindTexture(GL_TEXTURE_2D, texture);
      glUseProgram(program);
      glBindVertexArray(vao);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      window->SwapBuffer();
      viewer::Window::PollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
  } catch (const viewer::WindowException &ex) {
    spdlog::error("Window error: {}", ex.what());
    return -1;
  } catch (...) {
    spdlog::critical("Unknown error");
    return -1;
  }

  return 1;
}
