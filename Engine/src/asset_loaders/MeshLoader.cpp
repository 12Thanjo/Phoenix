#include "MeshLoader.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>



#include "Logging.h"
#include <format>


#include "vulkan/Renderer.h"


namespace ph{
	namespace assets{


		auto MeshLoader::init() noexcept -> bool {

			PH_INFO("Initialized: Mesh Loader (tinyobj)");
			return true;
		};



		auto MeshLoader::shutdown() noexcept -> void {

			PH_INFO("Shutdown: Mesh Loader");
		};



		
		// TODO: remove and replace with Assimp
		auto MeshLoader::load_mesh(const char* filepath) noexcept -> std::optional< std::pair<std::vector<vulkan::Vertex3D>, std::vector<uint32_t>> >{
			// https://vulkan-tutorial.com/Loading_models
			auto attrib          = tinyobj::attrib_t{};
			auto shapes          = std::vector<tinyobj::shape_t>{};
			auto materials       = std::vector<tinyobj::material_t>{};
			auto warning_message = std::string{};
			auto error_message   = std::string{};

			if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warning_message, &error_message, filepath)){
				PH_ERROR(std::format("tinyobj error: '{}'", error_message));
				PH_FATAL("Failed to load model");
				return std::nullopt;
			}

			if(warning_message.empty() == false){
				PH_WARNING(std::format("tinyobj warning: '{}'", warning_message));
			}


			auto unique_vertices = std::unordered_map<vulkan::Vertex3D, uint32_t>{};

			auto vertices = std::vector<vulkan::Vertex3D>{};
			auto indices = std::vector<uint32_t>{};

			for (const auto& shape : shapes) {
				for (const auto& index : shape.mesh.indices) {
					auto vertex = vulkan::Vertex3D{};

					vertex.position = {
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.tex_coord = {
						attrib.texcoords[2 * index.texcoord_index + 0],
						1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
					};

					vertex.normal = {
						attrib.normals[3 * index.normal_index + 0],
						attrib.normals[3 * index.normal_index + 1],
						attrib.normals[3 * index.normal_index + 2]
					};


					if(unique_vertices.count(vertex) == 0){
						unique_vertices[vertex] = uint32_t(vertices.size());
						vertices.push_back(vertex);
					}

					indices.push_back(unique_vertices[vertex]);
				}
			}


			return std::pair{std::move(vertices), std::move(indices)};
		};


	};
};