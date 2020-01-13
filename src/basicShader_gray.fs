#version 450

out vec4 color;

uniform sampler2D gray_texture;
uniform sampler2D depth_map;
in vec2 tex_coord0;
in vec3 normal0;
in vec4 vertex_position_light;


float evaluate_shadow(vec4 vertex_position_light) {
	float epsilon = 0.005;
	
	float shadow = texture2D(depth_map, vertex_position_light.xy).r;
	if (shadow + epsilon < vertex_position_light.z)		// jeżeli shadow jest dalej niż vertex_pos
		return 0.2;
	return 1.;	
}



void main() {
	vec4 vps = vertex_position_light;
	vps.xyz /= vertex_position_light.w;
	vps.xyz += 1.0;
	vps.xyz *= 0.5;
	float shadow_factor = evaluate_shadow(vps);
	
	
	//gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
/// 	color = vec4(1.0, 0.0, 0.0, 1.0);
	vec3 lightDirection = vec3(0,0,1);
	
	gl_FragColor = texture2D(gray_texture, tex_coord0 ) * clamp( dot(-lightDirection, normal0), 0.0, 1.0); 

///   	gl_FragColor = vec4 ( vec3(clamp( dot(-lightDirection, normal0), 0.0, 1.0) ), 1.0);
	
	
 	gl_FragColor *= shadow_factor;
	
}

