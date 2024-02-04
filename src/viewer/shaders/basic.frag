#version 450
#extension GL_ARB_separate_shader_objects : enable

layout (binding = 0) uniform sampler2D u_tex;

layout (location = 2) in vec2 v_uv;

layout (location = 0) out vec4 frag_color;

void main() {
  frag_color = texture(u_tex, v_uv);
}
