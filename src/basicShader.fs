#version 450

out vec4 color;

uniform sampler2D texture;
uniform sampler2D depth_map;
in vec2 tex_coord0;
in vec3 normal0;
in vec4 vertex_position_light;
in vec3 vertex_position_eye;
in vec3 lp_eye;

float evaluate_shadow(vec4 vertex_position_light) {
	float epsilon = 0.005;
	
	float shadow = texture2D(depth_map, vertex_position_light.xy).r;
	if (shadow + epsilon < vertex_position_light.z)						// jeżeli shadow jest dalej niż vertex_pos
		return 0.;
	return 1.;	
}



void main() {
	vec4 vps = vertex_position_light;
	vps.xyz /= vertex_position_light.w;
	vps.xyz += 1.0;
	vps.xyz *= 0.5;
	float shadow_factor = evaluate_shadow(vps);
	
	
	vec3 vertex_light = lp_eye - vertex_position_eye; // wektor pozycja--światło
/// 	vec3 lightDirection = normalize(vec3(2,2,0));
	vec3 lightDirection = normalize( vertex_light );

	float light_factor = dot(lightDirection, normal0);
	light_factor = max(light_factor, 0.0);
	
	vec3 directon_to_earth = vec3(0. ,0. ,1.);
	float mi_0 = dot(directon_to_earth, normal0);
	mi_0 = max(mi_0, 0.0);
	
	// (1.0 - LSL)*(dot_prod)/(dot_prod + dot_prod_2)+LSL*dot_prod;
	float LSL = 0.1;
	float jasnosc = (1.0 - LSL) * light_factor / (light_factor + mi_0) + LSL * light_factor;

/// 	gl_FragColor = vec4( light_factor , light_factor* shadow_factor, light_factor* shadow_factor, 1.0);
/// 	gl_FragColor = vec4( light_factor , light_factor, light_factor, 1.0) * shadow_factor;
	
	gl_FragColor = vec4( jasnosc , jasnosc, jasnosc, 1.0) * shadow_factor;
	
	
///   	gl_FragColor = texture2D(texture, tex_coord0 ) * clamp( dot(-lightDirection, normal0), 0.0, 1.0); 
///   	gl_FragColor = vec4 ( vec3( clamp( dot(lightDirection, normal0), 0.0, 1.0) ), 1.0);
///  	gl_FragColor *= shadow_factor;
	
}

