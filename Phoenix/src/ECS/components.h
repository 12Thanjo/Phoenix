#pragma once

#include "ScriptableEntity.h"

namespace Phoenix::Component{


	struct Name{
		std::string name;

		Name() = default;
		Name(const std::string& _name)
			: name(_name) {}
	};


	struct Transform{
		glm::vec3 translation = {0.0f, 0.0f, 0.0f};
		glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
		glm::vec3 scale = {100.0f, 100.0f, 100.0f};
		glm::mat4 transform{1.0f};

		Transform() = default;
		Transform(const glm::vec3& _translation, const glm::vec3& _rotation, const glm::vec3& _scale)
			: translation(_translation), rotation(_rotation), scale(_scale) {};
		Transform(const glm::mat4 & _transform)
			: transform(_transform) {};

	};



	struct SpriteRenderer{
		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRenderer() = default;
		SpriteRenderer(const glm::vec4& _color)
			: color(_color) {}

	};




	struct NativeScript{
		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*instantiate_script)();
		void (*destroy_script)(NativeScript*);

		template<typename T>
		void bind(){
			instantiate_script = [](){
				return static_cast<ScriptableEntity*>(new T());
			};

			destroy_script = [](NativeScript* native_script_component){
				delete native_script_component->instance;
				native_script_component->instance = nullptr;
			};
		}
	};
	
	
}
