#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


#include <optional>

namespace ph{
	namespace vulkan{

		EVO_NODISCARD auto create_image_view(
			const class Device& device, const VkImage& image, VkFormat format, VkImageAspectFlags aspect_flags, uint32_t mip_levels
		) noexcept -> std::optional<VkImageView>;


		EVO_NODISCARD auto create_image_sampler(const class Device& device, uint32_t mip_levels) noexcept -> std::optional<VkSampler>;

		auto destroy_image_sampler(const class Device& device, VkSampler& sampler) noexcept -> void;


		//////////////////////////////////////////////////////////////////////
		// image
		
		class Image{
			public:
				Image() = default;
				~Image() = default;


				EVO_NODISCARD auto create(
					const class Device& device,
					VkMemoryPropertyFlags properties,
					uint32_t image_width,
					uint32_t image_height,
					VkFormat format,
					VkImageUsageFlags usage,
					uint32_t image_mip_levels,
					VkSampleCountFlagBits num_samples,
					VkImageTiling tiling,
					VkImageAspectFlags view_aspect_flags
				) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;


				EVO_NODISCARD auto transition_layout(
					const class Device& device,
					const VkCommandPool& command_pool,
					const VkQueue& queue,
					VkImageLayout old_layout,
					VkImageLayout new_layout
				) noexcept -> bool;


				EVO_NODISCARD auto copy_from_buffer(
					const class Device& device, const VkCommandPool& command_pool, const VkQueue& queue, const class Buffer& buffer
				) noexcept -> bool;



				EVO_NODISCARD auto generate_mipmaps(
					const class Device& device,
					const VkCommandPool& command_pool,
					const VkQueue& queue,
					VkFormat format
				) noexcept -> bool;

		
			// TODO: private
			public:
				VkImage handle{VK_NULL_HANDLE};
				VkDeviceMemory memory{VK_NULL_HANDLE};
				VkImageView view{VK_NULL_HANDLE};


				uint32_t width;
				uint32_t height;
				uint32_t mip_levels;
		};


		//////////////////////////////////////////////////////////////////////
		// texture


		class Texture{
			public:
				Texture() = default;
				~Texture() = default;

				EVO_NODISCARD auto create(
					const class Device& device,
					const VkCommandPool& command_pool,
					const evo::ArrayProxy<byte> data,
					uint32_t width,
					uint32_t height,
					bool create_mipmaps
				) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;

				EVO_NODISCARD inline auto get_image_view() const noexcept -> const VkImageView& { return this->image.view; };
				EVO_NODISCARD inline auto get_sampler() const noexcept -> const VkSampler& { return this->sampler; };
		
			public:
				Image image{};
				VkSampler sampler{VK_NULL_HANDLE};
		};


	};
};