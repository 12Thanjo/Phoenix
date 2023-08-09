#version 450


layout(set = 1, binding = 0) uniform InstanceUBO {
    vec4 color;
} material;


layout(set = 1, binding = 1) uniform sampler2D texture_sampler;



layout(location = 0) in struct Transfer {
    vec2 tex_coord;
} transfer;

layout(location = 0) out vec4 out_color;

void main(){
	// out_color = vec4(transfer.tex_coord.x, 0.0, transfer.tex_coord.y, 1.0);
    out_color = texture(texture_sampler, transfer.tex_coord) * material.color;
}