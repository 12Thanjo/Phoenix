#version 450

struct DirectionalLight{
    vec3 direction;
    vec3 diffuse;
    float strength;
};



layout(set = 1, binding = 0) uniform InstanceUBO {
    vec4 color;
} material;


layout(set = 1, binding = 1) uniform sampler2D texture_sampler;




layout(location = 0) in struct Transfer {
    vec2 tex_coord;
    vec3 normal;
} transfer;

layout(location = 0) out vec4 out_color;


vec3 calcluate_lighting(DirectionalLight light, vec3 ambient, vec3 normal){

    // diffuse
    vec3 light_dir = normalize(light.direction);
    float light_normal_dir = dot(normal, light_dir);
    float diffuse_strength = max(light_normal_dir, 0.0);
    vec3 diffuse = light.diffuse * diffuse_strength;

    return (ambient + diffuse) * light.strength;
}




void main(){
    vec4 base_color = texture(texture_sampler, transfer.tex_coord) * material.color;

    DirectionalLight sun = DirectionalLight(
        vec3(0.1, 1.0, 0.3),
        vec3(0.9, 0.9, 0.95),
        0.4
    );


    out_color = vec4(calcluate_lighting(sun, base_color.rgb, transfer.normal), base_color.a);
}
