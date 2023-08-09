#version 450


layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_tex_coord;


layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 view;
    mat4 proj;
} global_ubo;


layout(push_constant) uniform PushConstants {
    mat4 model;
} push_constant;


layout(location = 0) out struct Transfer {
    vec2 tex_coord;
} transfer;


void main(){
    transfer.tex_coord = a_tex_coord;
	gl_Position = global_ubo.proj * global_ubo.view * push_constant.model * vec4(a_position, 0.0, 1.0);
}
