#pragma once

#include "../../include/Phoenix/defines.h"

#include "./EngineInterface.h"
#include "./common.h"

#include <libs/glm/glm.h>

#include <optional>
#include <string>

namespace ph{
	
	class AssetManager{
		public:
			AssetManager(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~AssetManager() = default;

			PH_NODISCARD inline auto loadMesh(const std::string& filepath) noexcept -> std::optional<Mesh3D> {
				Mesh3D out_id;
				if(this->interface->load_mesh(filepath.c_str(), &out_id) == false){
					return std::nullopt;
				}
				return out_id;
			};

			PH_NODISCARD inline auto loadTexture(const std::string& filepath) noexcept -> std::optional<TextureID> {
				TextureID out_id;
				if(this->interface->load_texture(filepath.c_str(), &out_id) == false){
					return std::nullopt;
				}
				return out_id;
			};


			inline auto setMaterialColor(Material3D material, glm::vec4 color) noexcept -> void {
				this->interface->set_material_color(material, &color);
			};

			inline auto setMaterialTexture(Material3D material, TextureID texture) noexcept -> void {
				this->interface->set_material_texture(material, texture);
			};


			inline auto setMaterialColor2D(Material2D material, glm::vec4 color) noexcept -> void {
				this->interface->set_material_color_2D(material, &color);
			};

			inline auto setMaterialTexture2D(Material2D material, TextureID texture) noexcept -> void {
				this->interface->set_material_texture_2D(material, texture);
			};



			PH_NODISCARD inline auto getCubeMesh() const noexcept -> Mesh3D { return this->interface->get_cube_mesh(); };
			PH_NODISCARD inline auto getDefaultTexture() const noexcept -> TextureID { return this->interface->get_default_texture(); };
	
		private:
			EngineInterface* interface;
	};

};
