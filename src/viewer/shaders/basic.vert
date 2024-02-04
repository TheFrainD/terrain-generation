#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv; 

layout (location = 2) out vec2 v_uv;

void main() {
  gl_Position = vec4(a_pos, 1.0);
  v_uv = a_uv;
}
