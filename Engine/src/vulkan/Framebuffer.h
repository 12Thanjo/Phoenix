#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


namespace ph{
	namespace vulkan{
		
		class Framebuffer{
			public:
				Framebuffer() = default;
				~Framebuffer() = default;

				EVO_NODISCARD auto create(
					const class Device& device,
					const class RenderPass& renderpass,
					uint32_t width,
					uint32_t height,
					const evo::ArrayProxy<VkImageView> attachments
				) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;

				EVO_NODISCARD constexpr auto is_valid() const noexcept -> bool { return this->handle != VK_NULL_HANDLE; };
		
			public:
				VkFramebuffer handle{VK_NULL_HANDLE};
		};


	};
};