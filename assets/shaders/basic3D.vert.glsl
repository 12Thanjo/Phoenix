#version 450

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_tex_coord;
layout(location = 2) in vec3 a_normal;


layout(set = 0, binding = 0) uniform GlobalUBO {
    mat4 view;
    mat4 proj;
} global_ubo;



layout(push_constant) uniform PushConstants {
    mat4 model;
} push_constant;



layout(location = 0) out struct Transfer {
    vec2 tex_coord;
    vec3 normal;
} transfer;


void main(){
    transfer.tex_coord = a_tex_coord;
    transfer.normal = normalize( (transpose( mat3(push_constant.model) )) * a_normal);

    gl_Position = global_ubo.proj * global_ubo.view * push_constant.model * vec4(a_position, 1.0);
}
