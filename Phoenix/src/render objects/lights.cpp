#include "ph_pch.h"
#include "lights.h"

#include "src/assets/AssetManager.h"

namespace Phoenix::Lights{
	
	//////////////////////////////////////////////////////////////////////
	// directional

	void Directional::upload(AssetManager* asset_manager, UUID& shader){
		asset_manager->uploadFloat3(shader, "u_directional_light.color", color);
		asset_manager->uploadFloat3(shader, "u_directional_light.direction", direction);
		asset_manager->uploadFloat(shader, "u_directional_light.strength", strength);

	}


	//////////////////////////////////////////////////////////////////////
	// point

	void Point::upload(AssetManager* asset_manager, UUID& shader, glm::vec3& position, int index){
		std::string index_str = std::to_string(index);
		asset_manager->uploadFloat3(shader, "u_point_lights[" + index_str  + "].ambient", ambient);
		asset_manager->uploadFloat3(shader, "u_point_lights[" + index_str  + "].diffuse", diffuse);
		asset_manager->uploadFloat3(shader, "u_point_lights[" + index_str  + "].specular", specular);

		asset_manager->uploadFloat3(shader, "u_point_lights[" + index_str  + "].position", position);
		asset_manager->uploadFloat(shader, "u_point_lights[" + index_str  + "].strength", strength);

		asset_manager->uploadFloat(shader, "u_point_lights[" + index_str  + "].linear", _linear);
		asset_manager->uploadFloat(shader, "u_point_lights[" + index_str  + "].quadratic", _quadratic);
	}

	void Point::setRange(float range){
		_range = range;
		_linear = 4.6905f * (float)pow(range, -1.01);
		_quadratic = 82.445f * (float)pow(range, -2.019);
	}
	

}
