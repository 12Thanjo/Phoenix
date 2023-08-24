#pragma once

#include "../../include/Phoenix/defines.h"

#include "./common.h"


#include <libs/glm/glm.h>


namespace ph{
	
	class PH_API EngineInterface{
		public:
			EngineInterface() noexcept;
			~EngineInterface() noexcept;


			using Callback = void(*)();


			///////////////////////////////////
			// engine

			PH_NODISCARD auto init() noexcept -> bool;
			auto shutdown() noexcept -> void;

			PH_NODISCARD auto execute() noexcept -> bool;


			auto set_update_callback(Callback callback) noexcept -> void;

			PH_NODISCARD auto get_frame_time() const noexcept -> float;


			///////////////////////////////////
			// inputs

			PH_NODISCARD auto is_mouse_down(int button) const noexcept -> bool;
			PH_NODISCARD auto is_mouse_up(int button) const noexcept -> bool;

			PH_NODISCARD auto was_mouse_down(int button) const noexcept -> bool;
			PH_NODISCARD auto was_mouse_up(int button) const noexcept -> bool;


			PH_NODISCARD auto is_key_down(int key) const noexcept -> bool;
			PH_NODISCARD auto is_key_up(int key) const noexcept -> bool;

			PH_NODISCARD auto was_key_down(int key) const noexcept -> bool;
			PH_NODISCARD auto was_key_up(int key) const noexcept -> bool;



			PH_NODISCARD auto mouse_x() const noexcept -> int32_t;
			PH_NODISCARD auto mouse_y() const noexcept -> int32_t;
			PH_NODISCARD auto mouse_dx() const noexcept -> int32_t;
			PH_NODISCARD auto mouse_dy() const noexcept -> int32_t;
			


			///////////////////////////////////
			// renderer

			auto set_render_callback_3D(Callback callback) noexcept -> void;
			auto set_render_callback_2D(Callback callback) noexcept -> void;

			auto create_material_3D(Material3D* out_material) noexcept -> bool;
			auto create_material_2D(Material2D* out_material) noexcept -> bool;


			auto bind_material(Material3D material) noexcept -> void;
			auto bind_material_2D(Material2D material) noexcept -> void;
			auto render_mesh(const glm::mat4& model, Mesh3D mesh) noexcept -> void;
			auto render_mesh_2D(const glm::mat4& model, glm::vec2 min_tex_coords, glm::vec2 max_tex_coords) noexcept -> void;
			// auto draw_text_3D(const char* string) noexcept -> void;
			auto draw_text_2D(const char* string, float font_size, glm::vec2 position) noexcept -> void;

			auto set_camera(const glm::mat4& transform) noexcept -> void;
			auto set_camera_2D(const glm::mat4& transform) noexcept -> void;


			///////////////////////////////////
			// asset manager

			PH_NODISCARD auto load_mesh(const char* filepath, Mesh3D* out_id) noexcept -> bool;
			PH_NODISCARD auto load_texture(const char* filepath, TextureID* out_id) noexcept -> bool;

			auto set_material_color(Material3D material, glm::vec4 color) noexcept -> void;
			auto set_material_texture(Material3D material, TextureID texture) noexcept -> void;

			auto set_material_color_2D(Material2D material, glm::vec4 color) noexcept -> void;
			auto set_material_texture_2D(Material2D material, TextureID texture) noexcept -> void;
			
			PH_NODISCARD auto get_cube_mesh() const noexcept -> Mesh3D;
			PH_NODISCARD auto get_default_texture() const noexcept -> TextureID;


			///////////////////////////////////
			// window

			PH_NODISCARD auto get_window_size(int* width, int* height) const noexcept -> void;

			auto capture_mouse() noexcept -> void;
			auto release_mouse() noexcept -> void;


			///////////////////////////////////
			// physics

			// static
			PH_NODISCARD auto create_static_cube(
				glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
			) noexcept -> StaticCollider;


			// dynamic
			PH_NODISCARD auto create_dynamic_cube(
				glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
			) noexcept -> DynamicCollider;


			auto set_dynamic_collider_density(DynamicCollider collider, float density) noexcept -> void;
			auto set_dynamic_collider_mass(DynamicCollider collider, float mass) noexcept -> void;

			PH_NODISCARD auto get_dynamic_collider_transform(DynamicCollider collider) const noexcept -> glm::mat4;


			// character controller
			PH_NODISCARD auto create_character_controller(glm::vec3 position, float height, float radius) noexcept -> CharacterController;
			PH_NODISCARD auto get_character_controller_position(CharacterController controller) const noexcept -> glm::vec3;
			auto character_controller_move(CharacterController controller, glm::vec3 direction, float dt) noexcept -> void;
			PH_NODISCARD auto is_character_controller_grounded(CharacterController controller) const noexcept -> bool;
			


		private:
			PH_NODISCARD auto draw_frame() noexcept -> bool;
			PH_NODISCARD auto resize() noexcept -> bool;
			PH_NODISCARD auto resize(uint32_t width, uint32_t height) noexcept -> bool;

			auto calculate_timings() noexcept -> void;
	
		private:
			struct EngineData* data = nullptr;
	};

};
