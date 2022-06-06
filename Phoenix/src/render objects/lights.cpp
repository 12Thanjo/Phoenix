#include "ph_pch.h"
#include "lights.h"

namespace Phoenix::Lights{
	
	void Directional::upload(Phoenix::Shader& shader){
		shader.uploadFloat3("u_directional_light.color", _color);
		shader.uploadFloat3("u_directional_light.direction", _direction);
		shader.uploadFloat("u_directional_light.strength", _strength);

	}



	void Point::upload(Phoenix::Shader& shader, int index){
		std::string index_str = std::to_string(index);
		shader.uploadFloat3("u_point_lights[" + index_str  + "].ambient", _ambient);
		shader.uploadFloat3("u_point_lights[" + index_str  + "].diffuse", _diffuse);
		shader.uploadFloat3("u_point_lights[" + index_str  + "].specular", _specular);

		shader.uploadFloat3("u_point_lights[" + index_str  + "].position", _position);
		shader.uploadFloat("u_point_lights[" + index_str  + "].strength", _strength);
	}


	

}
