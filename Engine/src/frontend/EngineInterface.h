#pragma once

#include "../../include/Phoenix/defines.h"

#include "./common.h"


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

			PH_NODISCARD auto is_key_down(int key) const noexcept -> bool;
			PH_NODISCARD auto is_key_up(int key) const noexcept -> bool;

			PH_NODISCARD auto mouse_x() const noexcept -> int32_t;
			PH_NODISCARD auto mouse_y() const noexcept -> int32_t;
			PH_NODISCARD auto mouse_dx() const noexcept -> int32_t;
			PH_NODISCARD auto mouse_dy() const noexcept -> int32_t;
			


			///////////////////////////////////
			// renderer

			auto set_render_callback_3D(Callback callback) noexcept -> void;
			auto set_render_callback_2D(Callback callback) noexcept -> void;

			auto bind_material(MaterialID id) noexcept -> void;
			auto bind_material_2D(MaterialID id) noexcept -> void;
			auto render_mesh(const alias::Mat4 model, MeshID id) noexcept -> void;
			auto render_mesh_2D(const alias::Mat4 model) noexcept -> void;

			auto set_camera(alias::Mat4 transform) noexcept -> void;
			auto set_camera_2D(alias::Mat4 transform) noexcept -> void;


			///////////////////////////////////
			// asset manager

			PH_NODISCARD auto load_mesh(const char* filepath, MeshID* out_id) noexcept -> bool;
			PH_NODISCARD auto load_texture(const char* filepath, TextureID* out_id) noexcept -> bool;

			auto set_material_color(MaterialID material, alias::Vec4 color) noexcept -> void;
			auto set_material_texture(MaterialID material, TextureID texture) noexcept -> void;

			auto set_material_color_2D(MaterialID material, alias::Vec4 color) noexcept -> void;
			auto set_material_texture_2D(MaterialID material, TextureID texture) noexcept -> void;
			
			PH_NODISCARD auto get_cube_mesh() const noexcept -> MeshID;
			PH_NODISCARD auto get_default_texture() const noexcept -> TextureID;


			///////////////////////////////////
			// window

			PH_NODISCARD auto get_window_size(int* width, int* height) const noexcept -> void;

			auto capture_mouse() noexcept -> void;
			auto release_mouse() noexcept -> void;


		private:
			PH_NODISCARD auto draw_frame() noexcept -> bool;
			PH_NODISCARD auto resize() noexcept -> bool;
			PH_NODISCARD auto resize(uint32_t width, uint32_t height) noexcept -> bool;

			auto calculate_timings() noexcept -> void;
	
		private:
			struct EngineBackend* backend = nullptr;
	};

};
