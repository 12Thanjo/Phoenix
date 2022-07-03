#include "ph_pch.h"
#include "lights.h"

#include "src/assets/AssetManager.h"

namespace Phoenix::Lights{
	
	void Directional::upload(AssetManager* asset_manager, UUID& shader){
		asset_manager->uploadFloat3(shader, "u_directional_light.color", color);
		asset_manager->uploadFloat3(shader, "u_directional_light.direction", direction);
		asset_manager->uploadFloat(shader, "u_directional_light.strength", strength);

	}



	void Point::upload(Phoenix::Shader& shader, int index){
		std::string index_str = std::to_string(index);
		shader.uploadFloat3("u_point_lights[" + index_str  + "].ambient", ambient);
		shader.uploadFloat3("u_point_lights[" + index_str  + "].diffuse", diffuse);
		shader.uploadFloat3("u_point_lights[" + index_str  + "].specular", specular);

		shader.uploadFloat3("u_point_lights[" + index_str  + "].position", position);
		shader.uploadFloat("u_point_lights[" + index_str  + "].strength", strength);
	}


	

}
