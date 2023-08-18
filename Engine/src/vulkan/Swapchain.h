#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


#include "vulkan/Framebuffer.h"


namespace ph{
	namespace vulkan{

		struct SwapChainSupportDetails{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats{};
			std::vector<VkPresentModeKHR> present_modes{};
		};

		EVO_NODISCARD auto query_swapchain_support(
			const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface
		) noexcept -> SwapChainSupportDetails;



		EVO_NODISCARD auto select_swapchain_surface_format(const evo::ArrayProxy<VkSurfaceFormatKHR> surface_formats) noexcept -> VkSurfaceFormatKHR;

		EVO_NODISCARD auto select_swapchain_present_mode(const evo::ArrayProxy<VkPresentModeKHR> present_modes) noexcept -> VkPresentModeKHR;

		EVO_NODISCARD auto select_swapchain_extent(
			const VkSurfaceCapabilitiesKHR& capabilities, uint32_t window_width, uint32_t window_height
		) noexcept -> VkExtent2D;

		//////////////////////////////////////////////////////////////////////
		// Swapchain
		
		class Swapchain{
			public:
				Swapchain() = default;
				~Swapchain() = default;


				EVO_NODISCARD auto create(
					const class Device& device,
					const VkSurfaceKHR& surface,
					uint32_t current_window_width,
					uint32_t current_window_height,
					const VkSwapchainKHR& old_swapchain = VK_NULL_HANDLE
				) noexcept -> bool;

				auto destroy(const class Device& device) noexcept -> void;


				EVO_NODISCARD auto resize(
					const class Device& device,
					const VkSurfaceKHR& surface,
					uint32_t new_width,
					uint32_t new_height
				) noexcept -> bool;



				enum class Result{
					Success,
					NeedResize,
					Error,
				};
				EVO_NODISCARD auto acquire_next_image(const class Device& device, const class Semaphore& image_available_semaphore) noexcept -> Result;

				EVO_NODISCARD auto present(const VkQueue& queue, const class Semaphore& render_finished_semaphore) noexcept -> Result;


				EVO_NODISCARD constexpr auto get_width() const noexcept -> uint32_t { return this->width; };
				EVO_NODISCARD constexpr auto get_height() const noexcept -> uint32_t { return this->height; };

				EVO_NODISCARD inline auto get_image_index() const noexcept -> uint32_t { return this->image_index; };
				EVO_NODISCARD constexpr auto get_image_format() const noexcept -> VkFormat { return this->image_format; };
				EVO_NODISCARD inline auto get_image_count() const noexcept -> uint32_t { return uint32_t(this->images.size()); };
				EVO_NODISCARD inline auto get_image_view(uint32_t index) const noexcept -> const VkImageView& {
					return this->image_views[index];
				};


			private:
				VkSwapchainKHR handle{VK_NULL_HANDLE};
				VkFormat image_format;

				std::vector<VkImage> images{};
				std::vector<VkImageView> image_views;

				uint32_t width;
				uint32_t height;

				uint32_t image_index;
		};


	};
};