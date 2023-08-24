#version 450


layout(set = 1, binding = 0) uniform InstanceUBO {
    vec4 color;
} material;


layout(set = 1, binding = 1) uniform sampler2D font_atlas;



layout(location = 0) in struct Transfer {
    vec2 tex_coord;
} transfer;

layout(location = 0) out vec4 out_color;




// https://github.com/Chlumsky/msdfgen/tree/d576034d226a5ceb8e8e1e94a09d94e58949cba1

float screenPxRange(){
    const float pixel_range = 2.0; // set to distance field's pixel range

    const vec2 unit_range = vec2(pixel_range, pixel_range) / textureSize(font_atlas, 0);
    const vec2 screen_tex_size = vec2(1.0, 1.0) / fwidth(transfer.tex_coord);
    return max(0.5 * dot(unit_range, screen_tex_size), 1.0);
}


float median(vec3 vec){
    return max(min(vec.r, vec.g), min(max(vec.r, vec.g), vec.b));
}


void main(){
	const vec3 msd = texture(font_atlas, transfer.tex_coord).rgb;
    const float sd = median(msd);
    const float screen_px_distance = screenPxRange() * (sd - 0.5);
    const float opacity = clamp(screen_px_distance + 0.5, 0.0, 1.0);

    if(opacity == 0.0){ discard; }

    out_color = mix(vec4(0.0, 0.0, 0.0, 0.0), material.color, opacity);
}