#pragma once

#include <libs/Vulkan/Vulkan.h>
#include <libs/glm/glm.h>
#include <libs/evo/evo.h>

#include <optional>




// TODO: change this? (Needed for format_size())
#include "Logging.h"

namespace ph{
	namespace vulkan{

		
		// TODO: change this value
		constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 1;
		constexpr uint32_t MAX_DESCRIPTOR_SETS = 100; // this was just arbitrarily chosen


		struct GlobalUBO3D{
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};

		struct GlobalUBO2D{
			alignas(16) glm::mat4 view;
			alignas(16) glm::mat4 proj;
		};


		struct InstanceUBO3D{
			glm::vec4 color;
		};

		struct InstanceUBO2D{
			glm::vec4 color;
		};


		//////////////////////////////////////////////////////////////////////
		// format

		template<typename T, unsigned count>
		constexpr auto format() noexcept -> VkFormat {
			static_assert(sizeof(T) == 0, "Cannot convert unsupported format");
		};

		template<> EVO_NODISCARD constexpr auto format<float, 1>() noexcept -> VkFormat { return VK_FORMAT_R32_SFLOAT; };
		template<> EVO_NODISCARD constexpr auto format<float, 2>() noexcept -> VkFormat { return VK_FORMAT_R32G32_SFLOAT; };
		template<> EVO_NODISCARD constexpr auto format<float, 3>() noexcept -> VkFormat { return VK_FORMAT_R32G32B32_SFLOAT; };
		template<> EVO_NODISCARD constexpr auto format<float, 4>() noexcept -> VkFormat { return VK_FORMAT_R32G32B32A32_SFLOAT; };

		template<> EVO_NODISCARD constexpr auto format<evo::byte, 1>() noexcept -> VkFormat { return VK_FORMAT_R8_UNORM; };
		template<> EVO_NODISCARD constexpr auto format<evo::byte, 2>() noexcept -> VkFormat { return VK_FORMAT_R8G8_UNORM; };
		template<> EVO_NODISCARD constexpr auto format<evo::byte, 3>() noexcept -> VkFormat { return VK_FORMAT_R8G8B8_UNORM; };
		template<> EVO_NODISCARD constexpr auto format<evo::byte, 4>() noexcept -> VkFormat { return VK_FORMAT_R8G8B8A8_UNORM; };



		EVO_NODISCARD constexpr auto format_size(VkFormat format) noexcept -> uint32_t {
			switch(format){
				case VK_FORMAT_R32_SFLOAT:          return (sizeof(float) * 1);
				case VK_FORMAT_R32G32_SFLOAT:       return (sizeof(float) * 2);
				case VK_FORMAT_R32G32B32_SFLOAT:    return (sizeof(float) * 3);
				case VK_FORMAT_R32G32B32A32_SFLOAT: return (sizeof(float) * 4);

				case VK_FORMAT_R8_UNORM:            return (sizeof(evo::byte) * 1);
				case VK_FORMAT_R8G8_UNORM:          return (sizeof(evo::byte) * 2);
				case VK_FORMAT_R8G8B8_UNORM:        return (sizeof(evo::byte) * 3);
				case VK_FORMAT_R8G8B8A8_UNORM:      return (sizeof(evo::byte) * 4);
			};

			PH_FATAL_BREAK("Unknown or unsupported format");
		};



		//////////////////////////////////////////////////////////////////////
		// result

		EVO_NODISCARD auto result_fail(VkResult result, const char* function) noexcept -> bool;



		//////////////////////////////////////////////////////////////////////
		// misc
		
		EVO_NODISCARD auto find_memory_type(
			const VkPhysicalDevice& physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties
		) noexcept -> std::optional<uint32_t>;


		// TODO:
		struct QueueFamilyIndices{
			std::optional<uint32_t> graphics_family;
			std::optional<uint32_t> present_family;
			std::optional<uint32_t> transfer_family;

			EVO_NODISCARD constexpr auto is_complete() const noexcept -> bool {
				return this->graphics_family.has_value() && this->present_family.has_value() && this->transfer_family.has_value();
			};
		};

		EVO_NODISCARD auto find_queue_families(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface) noexcept -> QueueFamilyIndices;

		EVO_NODISCARD auto print_physical_device_properties(const VkPhysicalDevice& physical_device) noexcept -> void;



		EVO_NODISCARD auto find_supported_format(
			const VkPhysicalDevice& physical_device, const evo::ArrayProxy<VkFormat> candidates, VkImageTiling tiling, VkFormatFeatureFlags features
		) noexcept -> VkFormat;

		EVO_NODISCARD auto find_depth_format(const VkPhysicalDevice& physical_device) noexcept -> VkFormat;

		EVO_NODISCARD auto format_has_stencil_component(VkFormat format) noexcept -> bool;



		EVO_NODISCARD auto get_max_msaa_samples(const VkPhysicalDevice& physical_device) noexcept -> VkSampleCountFlagBits;



		EVO_NODISCARD auto load_shader_code(const std::string& filepath) noexcept -> std::optional< std::vector<evo::byte> >;



		struct Vertex3D{
			glm::vec3 position;
			glm::vec2 tex_coord;
			glm::vec3 normal;


			EVO_NODISCARD constexpr auto operator==(const Vertex3D& other) const noexcept -> bool {
				return (this->position == other.position) && (this->tex_coord == other.tex_coord) && (this->normal == other.normal);
			};
		};


		struct Vertex2D{
			glm::vec2 position;
			glm::vec2 tex_coord;


			EVO_NODISCARD constexpr auto operator==(const Vertex2D& other) const noexcept -> bool {
				return (this->position == other.position) && (this->tex_coord == other.tex_coord);
			};
		};

	};
};



namespace std{
	

	template<>
	struct hash<ph::vulkan::Vertex3D>{

		EVO_NODISCARD inline auto operator()(const ph::vulkan::Vertex3D& vertex) const noexcept -> size_t {
			const auto position_hash = hash<glm::vec3>()(vertex.position);
			const auto tex_coord_hash = hash<glm::vec2>()(vertex.tex_coord);

			return ((position_hash ^ (tex_coord_hash << 1)) >> 1);
		};

	};


	template<>
	struct hash<ph::vulkan::Vertex2D>{

		EVO_NODISCARD inline auto operator()(const ph::vulkan::Vertex2D& vertex) const noexcept -> size_t {
			const auto position_hash = hash<glm::vec2>()(vertex.position);
			const auto tex_coord_hash = hash<glm::vec2>()(vertex.tex_coord);

			return ((position_hash ^ (tex_coord_hash << 1)) >> 1);
		};

	};


};