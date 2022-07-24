#pragma once

#include "src/render objects/cameras.h"
#include "core/UUID.h"
#include "Entity.h"

#include "src/scripting/scripting.h"

#include "src/assets/materials.h"


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
		Script(const std::string& script_path, const std::string& full_path, Scripting& scripting, Phoenix::UUID uuid)
			: path(script_path) {
				
			add_script(scripting, full_path, uuid);
		};


		private:
			void add_script(Scripting& scripting, const std::string& full_path, Phoenix::UUID uuid){
				std::string script = Files::readFile(full_path);

				std::string script_path = path;
				std::string from = "\\";
				std::string to = "\\\\";
				size_t start_pos = 0;
				while((start_pos = script_path.find(from, start_pos)) != std::string::npos) {
				    script_path.replace(start_pos, from.length(), to);
				    start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
				}

				scripting.createScript(script, script_path);
			}
	};


	//////////////////////////////////////////////////////////////////////
	// cameras

	struct PerspectiveCamera{
		Phoenix::PerspectiveCamera camera;

		PerspectiveCamera(float fov, float aspect_ratio, float near, float far)
			: camera(fov, aspect_ratio, near, far) {};
	};

	struct OrbitalCamera{
		Phoenix::OrbitalCamera camera;

		OrbitalCamera(float fov, float aspect_ratio, float near, float far)
			: camera(fov, aspect_ratio, near, far) {};
	};




	//////////////////////////////////////////////////////////////////////
	// rendering

	struct Sprite{
		glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

		bool using_texture = false;
		bool has_texture = false;
		Phoenix::UUID texture;

		Sprite() = default;
		Sprite(const glm::vec4& _color)
			: color(_color) {}
	};



	struct Material{
		BasicMaterial material;

		// this is just for the editor
		bool using_texture = false;
		bool has_save_texture = false;
		Phoenix::UUID save_texture;

		Material() = default;
		Material(const BasicMaterial& _material)
			: material(_material) {}
	};


	struct Cube : public Material{};
	struct Plane : public Material{};
	
}
