#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec2 tex_coord0;
out vec3 normal0;
out vec4 vertex_position_light;
out vec3 vertex_position_eye;
out vec3 lp_eye;

uniform mat4 model_matrix, view_matrix, projection_matrix;
uniform mat4 light_projection_matrix, light_view_matrix;

uniform vec3 light_position;

void main() {
	// do cienia: pozycje z pkt.widz. źródła światła
	vertex_position_light = light_projection_matrix * light_view_matrix * model_matrix * vec4(vertex_position, 1.0);
	
	
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertex_position, 1.0);
	vertex_position_eye = vec3(view_matrix * model_matrix * vec4(vertex_position, 1.0) );
	tex_coord0 = tex_coord;
	normal0 = vec3(view_matrix * model_matrix * vec4(normal, 0.0));			// 0 znaczy, ze ostatnia kolumna macierzy jest ignorowana -> brak przesnięcia
	normal0 = normalize(normal0);

	vec3 lp = light_position;
	//lp.x = light_position[0]; lp.y = light_position[1]; lp.z = light_position[2];
	
//	lp_eye = vec3(view_matrix * vec4(2., 2., 0., 1.0) );
	lp_eye = vec3(view_matrix * vec4(lp, 1.0) );
	
//	lp_eye = vec3(0., 0., 6.);
}

