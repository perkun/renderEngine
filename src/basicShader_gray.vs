#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec2 tex_coord0;
out vec3 normal0;
out vec4 vertex_position_light;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform mat4 light_projection_matrix, light_view_matrix;


void main() {
	// do cienia: pozycje z pkt.widz. źródła światła
	vertex_position_light = light_projection_matrix * light_view_matrix * model_matrix * vec4(vertex_position, 1.0);
	
	
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);
	tex_coord0 = tex_coord;
	normal0 = (projection_matrix * view_matrix * model_matrix * vec4(normal, 0.0)).xyz;			// 0 znaczy, ze ostatnia kolumna macierzy jest ignorowana -> brak przesnięcia
	normal0 = normalize(normal0);	
}

