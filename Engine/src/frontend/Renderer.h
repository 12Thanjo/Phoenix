#pragma once

#include "../../include/Phoenix/defines.h"

#include "./EngineInterface.h"

#include "./common.h"

#include <libs/glm/glm.h>

#include <functional>

namespace ph{

	//////////////////////////////////////////////////////////////////////
	// 3D
	
	class Renderer3D{
		public:
			Renderer3D(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~Renderer3D() = default;


			inline auto setCallback(const std::function<void()> callback) noexcept -> void {
				const static auto callback_func = callback;

				this->interface->set_render_callback_3D([](){
					callback_func();
				});
			};

			inline auto createMaterial() noexcept -> std::optional<Material3D> {
				Material3D output;

				if(this->interface->create_material_3D(&output) == false){ return std::nullopt; }

				return output;
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



			inline auto createMaterial() noexcept -> std::optional<Material2D> {
				Material2D output;

				if(this->interface->create_material_2D(&output) == false){ return std::nullopt; }

				return output;
			};

			inline auto bindMaterial(Material2D material) noexcept -> void { this->interface->bind_material_2D(material); };

			inline auto drawQuad(
				const glm::mat4& model,
				glm::vec2 min_tex_coords = {0.0f, 0.0f},
				glm::vec2 max_tex_coords = {1.0f, 1.0f}
			) noexcept -> void {
				this->interface->render_mesh_2D(model, min_tex_coords, max_tex_coords);
			};


			inline auto drawText(const std::string& string, float font_size, glm::vec2 position) noexcept -> void {
				this->interface->draw_text_2D(string.c_str(), font_size, position);
			};

			inline auto setCamera(const glm::mat4& transform) noexcept -> void {
				this->interface->set_camera_2D(transform);
			};

	
		private:
			EngineInterface* interface;
	};

};
