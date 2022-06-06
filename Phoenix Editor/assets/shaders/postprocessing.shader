#shader vertex
#version 450 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texture_coordinates;

out vec2 t_texture_coordinates;


void main(){
	t_texture_coordinates = a_texture_coordinates;
	gl_Position = vec4(a_position, 0.0, 1.0);
}



#shader fragment
#version 450 core

out vec4 frag_color;

in vec2 t_texture_coordinates;

uniform sampler2D u_texture;


void main(){
	vec4 base = texture(u_texture, t_texture_coordinates);
	// base.r *= 0.4;
	// base.g *= 0.4;



	

	frag_color = base;
}