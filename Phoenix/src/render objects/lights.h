#pragma once
#include "../assets/shaders.h"

namespace Phoenix{
	class AssetManager;
}

namespace Phoenix::Lights{
	static glm::vec3 sunrise  = glm::vec3(0.713f, 0.494f, 0.357f);
	static glm::vec3 noon     = glm::vec3(0.753f, 0.749f, 0.678f);
	static glm::vec3 haze     = glm::vec3(0.741f, 0.745f, 0.753f);
	static glm::vec3 overcast = glm::vec3(0.682f, 0.718f, 0.745f);

	
	class Directional{
		public:
			Directional() = default;
			~Directional() = default;

			void upload(AssetManager* asset_manager, UUID& shader);

			glm::vec3 color = noon;
			glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
			float strength = 1.3f;
	};




	class Point{
		private:
	
		public:
			Point(glm::vec3 light_position) : position(light_position) {};
			~Point() = default;
			
			void upload(Phoenix::Shader& shader, int index);


			glm::vec3 ambient = noon;
			glm::vec3 diffuse = noon;
			glm::vec3 specular = noon;

			glm::vec3 position;
			float strength = 1.0f;
	};

}
