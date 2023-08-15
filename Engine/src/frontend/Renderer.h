#pragma once

#include "../../include/Phoenix/defines.h"

#include "./EngineInterface.h"

#include "./common.h"

#include <libs/glm/glm.h>

#include <functional>

namespace ph{
	
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


			inline auto setCallback2D(const std::function<void()> callback) noexcept -> void {
				const static auto callback_func = callback;

				this->interface->set_render_callback_2D([](){
					callback_func();
				});
			};


			inline auto bindMaterial(Material3D material) noexcept -> void { this->interface->bind_material(material); };
			inline auto drawMesh(const glm::mat4& model, Mesh3D id) noexcept -> void { this->interface->render_mesh(model, id); };

			inline auto bindMaterial2D(Material2D material) noexcept -> void { this->interface->bind_material_2D(material); };
			inline auto renderQuad2D(const glm::mat4& model) noexcept -> void { this->interface->render_mesh_2D(model); };

			inline auto setCamera(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera(transform);
			};

			inline auto setCamera2D(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera_2D(transform);
			};

	
		private:
			EngineInterface* interface;
	};

};
