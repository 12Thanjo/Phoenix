#shader vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texture_coordinates;
layout(location = 2) in vec3 a_normal;

out vec2 t_texture_coordinates;
out vec3 t_fragment_position;
out vec3 t_normal;


// uniform sampler2D u_texture;
uniform mat4 u_model;
uniform mat4 u_view_projection;
// uniform mat4 u_view;
// uniform mat4 u_projection;

void main(){
	t_texture_coordinates = a_texture_coordinates;
	t_fragment_position = vec3(u_model * vec4(a_position, 1.0));
	t_normal = inverse(transpose(mat3(u_model))) * a_normal;
	


	// gl_Position = u_projection * u_view * vec4(t_fragment_position, 1.0);
	// gl_Position = u_view_projection * vec4(t_fragment_position, 1.0);	
	gl_Position = u_view_projection * u_model * vec4(a_position, 1.0);
}



#shader fragment
#version 450 core


struct PointLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	float strength;

	float linear;
	float quadratic;
};


struct DirectionalLight{
	vec3 color;
	vec3 direction;
	float strength;
};



layout(location=0) out vec4 frag_color;
layout(location=1) out int entity_id;

in vec2 t_texture_coordinates;
in vec3 t_fragment_position;
in vec3 t_normal;


uniform vec4 u_color;
uniform sampler2D u_texture;
uniform int u_using_texture;
uniform float u_shininess;
uniform vec3 u_camera_position;

uniform int u_num_point_lights;
uniform PointLight u_point_lights[16];
uniform DirectionalLight u_directional_light;

uniform int u_entity_id;



vec3 calculate_point_light(PointLight light){
	// ambient
	vec3 ambient = 0.1 * light.ambient;

	// diffuse
	vec3 norm = normalize(t_normal);
	vec3 light_direction = normalize(light.position - t_fragment_position);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light.diffuse;


	// specular
	vec3 view_direction = normalize(u_camera_position - t_fragment_position);
	vec3 reflect_direction = reflect(-light_direction, norm);
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_shininess);
	vec3 specular = 1.5 * spec * light.specular;


	// attenuation
	float distance = length(light.position - t_fragment_position);
	float attenuation = 1.0 / ((1 / light.strength) + light.linear * distance + light.quadratic * (distance * distance));


	// ambient  *= attenuation;
	// diffuse  *= attenuation;
	// specular *= attenuation;


	// total
	return attenuation * (ambient + diffuse + specular);
}


vec3 calculate_directional_light(DirectionalLight light){
	vec3 ambient = 0.1 * light.color;

	// diffuse
	vec3 norm = normalize(t_normal);
	// vec3 light_direction = normalize(light.position - t_fragment_position);
	vec3 light_direction = normalize(-light.direction);
	float diff = max(dot(norm, light_direction), 0.0);
	vec3 diffuse = diff * light.color;


	// specular
	vec3 specular = vec3(0, 0, 0);
	if(u_shininess > 1){
		vec3 view_direction = normalize(u_camera_position - t_fragment_position);
		vec3 reflect_direction = reflect(-light_direction, norm);
		float spec = pow(max(dot(view_direction, reflect_direction), 0.0), u_shininess);
		specular = 1.5 * spec * light.color;
	}



	// total
	return light.strength * (ambient + diffuse + specular);
}




void main(){
	vec3 lighting = calculate_directional_light(u_directional_light);


	for(int i = 0; i < u_num_point_lights; i += 1){
		lighting += calculate_point_light(u_point_lights[i]);
	}


	if(u_using_texture == 1){
		frag_color = vec4(lighting, 1.0)  * u_color * texture(u_texture, t_texture_coordinates);
	}else{
		frag_color = vec4(lighting, 1.0) * u_color;
	}

	entity_id = u_entity_id;
}