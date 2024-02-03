#include <GL/glew.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <fstream>
#include <memory>
#include <vector>

#include "window.h"

int main(int argc, char **argv) {
  spdlog::set_level(spdlog::level::trace);

  try {
    auto window {std::make_unique<viewer::Window>(640, 480, "Viewer")};

    float vertices[] = {
        0.5f,  0.5f,  0.0f,  // top right
        0.5f,  -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    const char *vertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char *fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n\0";

    std::ifstream vertex_shader_file("shaders/basic.vert.spv", std::ios::binary);
    std::ifstream fragment_shader_file("shaders/basic.frag.spv", std::ios::binary);

    std::vector<char> vertex_shader_buffer((std::istreambuf_iterator<char>(vertex_shader_file)),
                                           (std::istreambuf_iterator<char>()));
    std::vector<char> fragment_shader_buffer((std::istreambuf_iterator<char>(fragment_shader_file)),
                                             (std::istreambuf_iterator<char>()));

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderBinary(1, &vertex_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertex_shader_buffer.data(),
                   vertex_shader_buffer.size());
    glSpecializeShader(vertex_shader, "main", 0, nullptr, nullptr);
    GLint success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
      spdlog::error("Vertex compile error: {}", info_log);
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderBinary(1, &fragment_shader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragment_shader_buffer.data(),
                   fragment_shader_buffer.size());
    glSpecializeShader(fragment_shader, "main", 0, nullptr, nullptr);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
      spdlog::error("Fragment compile error: {}", info_log);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(program, 512, nullptr, info_log);
      spdlog::error("Linking error: {}", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!window->ShouldClose()) {
      glClearColor(0.2, 0.3, 0.3, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
