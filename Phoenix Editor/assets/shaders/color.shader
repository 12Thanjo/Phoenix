#shader vertex
#version 450 core

layout(location = 0) in vec2 a_position;


uniform mat4 u_model;


void main(){
	gl_Position = u_model * vec4(a_position, 0.0, 1.0);
}



#shader fragment
#version 450 core


out vec4 frag_color;

uniform vec4 u_color;


void main(){
	frag_color = u_color;
}