#pragma once

#include "../../include/Phoenix/defines.h"

#include "./EngineInterface.h"

#include "./common.h"

#include <libs/glm/glm.h>

#include <functional>

namespace ph{

	//////////////////////////////////////////////////////////////////////
	// 3D
	
	class Renderer{
		public:
			Renderer(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~Renderer() = default;


			inline auto setCallback3D(const std::function<void()> callback) noexcept -> void {
				const static auto callback_func = callback;

				this->interface->set_render_callback_3D([](){
					callback_func();
				});
			};


			inline auto bindMaterial(Material3D material) noexcept -> void { this->interface->bind_material(material); };
			inline auto drawMesh(const glm::mat4& model, Mesh3D id) noexcept -> void { this->interface->render_mesh(model, id); };


			inline auto setCamera(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera(transform);
			};


	
		private:
			EngineInterface* interface;
	};



	//////////////////////////////////////////////////////////////////////
	// 2D

	class Renderer2D{
		public:
			Renderer2D(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~Renderer2D() = default;

			inline auto setCallback(const std::function<void()> callback) noexcept -> void {
				const static auto callback_func = callback;

				this->interface->set_render_callback_2D([](){
					callback_func();
				});
			};



			inline auto bindMaterial(Material2D material) noexcept -> void { this->interface->bind_material_2D(material); };
			inline auto drawQuad(const glm::mat4& model) noexcept -> void { this->interface->render_mesh_2D(model); };

			inline auto setCamera(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera_2D(transform);
			};

	
		private:
			EngineInterface* interface;
	};

};
