#shader vertex
#version 450 core

layout(location = 0) in vec2 a_position;


uniform mat4 u_model;


void main(){
	gl_Position = u_model * vec4(a_position, 0.0, 1.0);
}



#shader fragment
#version 450 core


layout(location=0) out vec4 frag_color;
layout(location=1) out int entity_id;

uniform vec4 u_color;
uniform int u_entity_id;


void main(){
	frag_color = u_color;
	entity_id = u_entity_id;
}