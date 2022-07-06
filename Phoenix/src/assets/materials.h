#pragma once


namespace Phoenix{

	class BasicMaterial{
		public:
			BasicMaterial(glm::vec4 _color = {1.0f, 1.0f, 1.0f, 1.0f}, float _shinyness = 32.0f)
				: color(_color), shinyness(_shinyness) {};
			~BasicMaterial() = default;
	
			
			glm::vec4 color;
			float shinyness;
			
	};

}