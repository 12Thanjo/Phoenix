#shader vertex
#version 450 core

layout (location = 0) in vec3 a_position;
// layout (location = 1) in vec2 a_texture_coordinates;

// out vec2 t_texture_coordinates;

uniform mat4 u_model;
uniform mat4 u_view_projection;

void main(){
	// t_texture_coordinates = a_texture_coordinates;
	// vec3 pos = vec3(u_model * vec4(a_position, 1.0));

	vec4 pos = u_view_projection * u_model * vec4(a_position, 1.0);
	
	gl_Position = pos;
}



#shader fragment
#version 450 core

out vec4 frag_color;

// in vec2 t_texture_coordinates;

// uniform sampler2D u_texture;


void main(){
	frag_color = vec4(0.0, 0.3, 0.5, 1.0);
}