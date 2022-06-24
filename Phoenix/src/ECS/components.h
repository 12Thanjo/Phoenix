#pragma once

#include "src/render objects/cameras.h"
#include "core/UUID.h"
#include "Entity.h"


namespace Phoenix::Component{

	struct UUID{
		Phoenix::UUID id;

		UUID() : id() {};
		UUID(const Phoenix::UUID& _id) : id(_id) {};
	};
	
	struct Name{
		std::string name;

		Name() = default;
		Name(const std::string& _name)
			: name(_name) {}
	};


	struct Transform{
		glm::vec3 position = {0.0f, 0.0f, 0.0f};
		glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f};
		glm::mat4 transform{1.0f};

		Transform() = default;
		Transform(const glm::vec3& _position, const glm::vec3& _rotation, const glm::vec3& _scale)
			: position(_position), rotation(_rotation), scale(_scale) {};
		Transform(const glm::mat4 & _transform)
			: transform(_transform) {};

	};


	struct Script{
		std::string path;

		Script() = default;
		Script(const std::string& script_path)
			: path(script_path) {};
	};


	//////////////////////////////////////////////////////////////////////
	// cameras

	struct PerspectiveCamera{
		Phoenix::PerspectiveCamera camera;

		PerspectiveCamera(float fov, float aspect_ratio, float near, float far)
			: camera(fov, aspect_ratio, near, far) {}
	};

	struct OrbitalCamera{
		Phoenix::OrbitalCamera camera;

		OrbitalCamera(float fov, float aspect_ratio, float near, float far)
			: camera(fov, aspect_ratio, near, far) {}
	};




	//////////////////////////////////////////////////////////////////////
	// rendering

	struct SpriteRenderer{
		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRenderer() = default;
		SpriteRenderer(const glm::vec4& _color)
			: color(_color) {}
	};


	struct Cube{
		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

		Cube() = default;
		Cube(const glm::vec4& _color)
			: color(_color) {}
	};
	
}
