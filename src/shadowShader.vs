#version 420

layout(location = 0) in vec3 vertex_position;
uniform mat4 light_projection_matrix, light_view_matrix, model_matrix;

void main () {
  gl_Position = light_projection_matrix * light_view_matrix * model_matrix * vec4 (vertex_position, 1.0);
}
