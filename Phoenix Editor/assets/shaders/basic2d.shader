#shader vertex
#version 450 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texture_coordinates;


uniform mat4 u_model;

out vec2 t_texture_coordinates;


void main(){
	t_texture_coordinates = a_texture_coordinates;

	gl_Position = u_model * vec4(a_position, 0.0, 1.0);
}



#shader fragment
#version 450 core


layout(location=0) out vec4 frag_color;
layout(location=1) out int entity_id;


in vec2 t_texture_coordinates;


uniform vec4 u_color;
uniform sampler2D u_texture;
uniform int u_using_texture;

uniform int u_entity_id;


void main(){
	if(u_using_texture == 1){
		frag_color = u_color * texture(u_texture, t_texture_coordinates);
	}else{
		frag_color = u_color;
	}

	entity_id = u_entity_id;
}