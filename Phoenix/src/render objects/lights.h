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

		public:
			glm::vec3 color = noon;
			glm::vec3 direction = glm::vec3(-1.0f, -1.0f, -1.0f);
			float strength = 1.3f;
	};




	class Point{
		public:
			Point() = default;
			~Point() = default;
			
			void upload(AssetManager* asset_manager, UUID& shader, glm::vec3& position, int index);

			void setRange(float range);
			inline float getRange() const { return _range; };
			inline float getLinear() const { return _linear; };
			inline float getQuadratic() const { return _quadratic; };

		public:
			glm::vec3 ambient = noon;
			glm::vec3 diffuse = noon;
			glm::vec3 specular = noon;

			float strength = 1.0f;
		

		private:
			float _range = 50.0f;
			float _linear = 0.090211f;
			float _quadratic = 0.0306157f;
	};

}
