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


			inline auto bindMaterial(MaterialID id) noexcept -> void { this->interface->bind_material(id); };
			inline auto drawMesh(const glm::mat4& model, MeshID id) noexcept -> void { this->interface->render_mesh((alias::Mat4)&model, id); };

			inline auto bindMaterial2D(MaterialID id) noexcept -> void { this->interface->bind_material_2D(id); };
			inline auto renderQuad2D(const glm::mat4& model) noexcept -> void { this->interface->render_mesh_2D((alias::Mat4)&model); };

			inline auto setCamera(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera((alias::Mat4)&transform);
			};

			inline auto setCamera2D(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera_2D((alias::Mat4)&transform);
			};

	
		private:
			EngineInterface* interface;
	};

};
