#include "Swapchain.h"

#include "Logging.h"

#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/images.h"
#include "vulkan/synchronization.h"

#include <algorithm>

namespace ph{
	namespace vulkan{


		auto query_swapchain_support(const VkPhysicalDevice& physical_device, const VkSurfaceKHR& surface) noexcept -> SwapChainSupportDetails {

			auto details = SwapChainSupportDetails{};


			///////////////////////////////////
			// capabilities

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);


			///////////////////////////////////
			// formats

			uint32_t format_count;
			vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, nullptr);

			if(format_count != 0){
				details.formats.resize(format_count);
				vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &format_count, details.formats.data());
			}


			///////////////////////////////////
			// present modes

			uint32_t present_mode_count;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, nullptr);

			if(present_mode_count != 0){
				details.present_modes.resize(present_mode_count);
				vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &present_mode_count, details.present_modes.data());
			}


			///////////////////////////////////
			// done

			return details;
		};




		auto select_swapchain_surface_format(const evo::ArrayProxy<VkSurfaceFormatKHR> surface_formats) noexcept -> VkSurfaceFormatKHR {
			for(auto& surface_format : surface_formats){
				if(surface_format.format == VK_FORMAT_B8G8R8A8_UNORM && surface_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
					return surface_format;
				}
			}

			// if none found that meet criteria, just pick the first one
			return surface_formats[0];
		};




		EVO_NODISCARD auto select_swapchain_present_mode(const evo::ArrayProxy<VkPresentModeKHR> present_modes) noexcept -> VkPresentModeKHR {
			// Look for mailbox
			for(auto& present_mode : present_modes){
				if(present_mode == VK_PRESENT_MODE_MAILBOX_KHR) return present_mode;
			}

			// else just use FIFO (guaranteed)
			return VK_PRESENT_MODE_FIFO_KHR;
		};


		EVO_NODISCARD auto select_swapchain_extent(
			const VkSurfaceCapabilitiesKHR& capabilities, uint32_t window_width, uint32_t window_height
		) noexcept -> VkExtent2D {

			if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
				return capabilities.currentExtent;
			};

			auto actual_extent = VkExtent2D{window_width, window_height};

			actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actual_extent;
		};


		//////////////////////////////////////////////////////////////////////
		// swapchain

		auto Swapchain::create(
			const Device& device,
			const VkSurfaceKHR& surface,
			uint32_t current_window_width,
			uint32_t current_window_height,
			const VkSwapchainKHR& old_swapchain
		) noexcept -> bool {

			const SwapChainSupportDetails swapchain_support = vulkan::query_swapchain_support(device.get_physical(), surface);

			const VkSurfaceFormatKHR surface_format = vulkan::select_swapchain_surface_format(swapchain_support.formats);
			const VkPresentModeKHR present_mode = vulkan::select_swapchain_present_mode(swapchain_support.present_modes);

			const VkExtent2D extent = vulkan::select_swapchain_extent(swapchain_support.capabilities, current_window_width, current_window_height);

			uint32_t image_count = swapchain_support.capabilities.minImageCount + 1;
			if(swapchain_support.capabilities.maxImageCount > 0 && image_count > swapchain_support.capabilities.maxImageCount){
				image_count = swapchain_support.capabilities.maxImageCount;
			}


			this->image_format = surface_format.format;
			this->width = extent.width;
			this->height = extent.height;



			const vulkan::QueueFamilyIndices indices = vulkan::find_queue_families(device.get_physical(), surface);
			if(indices.is_complete() == false){
				PH_FATAL("Failed to find Vulkan queue families for swapchain");
				return false;
			}
			const auto queue_family_indicies = std::array<uint32_t, 2>{*indices.graphics_family, *indices.present_family};


			const bool queue_families_different = (indices.graphics_family != indices.present_family);


			///////////////////////////////////
			// swapchain

			auto create_info = VkSwapchainCreateInfoKHR{ .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
				.surface               = surface,
				.minImageCount         = image_count,
				.imageFormat           = surface_format.format,
				.imageColorSpace       = surface_format.colorSpace,
				.imageExtent           = extent,
				.imageArrayLayers      = 1,
				.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

				.imageSharingMode      = queue_families_different ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
				.queueFamilyIndexCount = queue_families_different ? static_cast<uint32_t>(queue_family_indicies.size()) : 0ul,
				.pQueueFamilyIndices   = queue_families_different ? queue_family_indicies.data() : nullptr,

				.preTransform          = swapchain_support.capabilities.currentTransform,
				.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				.presentMode           = present_mode,
				.clipped               = VK_TRUE,
				.oldSwapchain		   = old_swapchain,
			};



			const VkResult result = vkCreateSwapchainKHR(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateSwapchainKHR")) return false;


			///////////////////////////////////
			// images

			vkGetSwapchainImagesKHR(device.get_handle(), this->handle, &image_count, nullptr);
			this->images.resize(image_count);
			vkGetSwapchainImagesKHR(device.get_handle(), this->handle, &image_count, this->images.data());



			///////////////////////////////////
			// image views

			this->image_views.resize(this->images.size());

			for(uint32_t i = 0; i < this->images.size(); i++){
				auto image_view_result = vulkan::create_image_view(device, this->images[i], this->image_format, VK_IMAGE_ASPECT_COLOR_BIT, 1);
				if(image_view_result.has_value() == false) return false;
					
				this->image_views[i] = std::move(*image_view_result);
			}


			///////////////////////////////////
			// done

			PH_DEBUG("Created: Vulkan Swapchain");
			return true;
		};




		auto Swapchain::destroy(const Device& device) noexcept -> void {
			for(auto& image_view : this->image_views){
				vkDestroyImageView(device.get_handle(), image_view, nullptr);
			}

			vkDestroySwapchainKHR(device.get_handle(), this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;



			PH_DEBUG("Destroyed: Vulkan Swapchain");
		};



		auto Swapchain::resize(
			const Device& device,
			const VkSurfaceKHR& surface,
			uint32_t new_width,
			uint32_t new_height
		) noexcept -> bool {

			auto new_swapchain = Swapchain{};

			if(new_swapchain.create(device, surface, new_width, new_height, this->handle) == false){
				PH_ERROR("Error when trying to create new swapchain (due to a resize)");
				return false;				
			}


			// TODO: might not need this
			if(device.wait_idle() == false){
				PH_FATAL("Failed to wait for device to become idle");
				return false;
			}


			this->destroy(device);

			*this = std::move(new_swapchain);

			PH_TRACE("Resized swapchain");
			return true;
		};





		auto Swapchain::acquire_next_image(const Device& device, const Semaphore& image_available_semaphore) noexcept -> Swapchain::Result {

			const VkResult result = vkAcquireNextImageKHR(
				device.get_handle(), this->handle, UINT64_MAX, image_available_semaphore.handle, VK_NULL_HANDLE, &this->image_index
			);

			if(result == VK_ERROR_OUT_OF_DATE_KHR){
				return Swapchain::Result::NeedResize;

			}else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
				PH_FATAL("Failed to acquire swapchain image");
				return Swapchain::Result::Error;
			}

			return Swapchain::Result::Success;
		};



		auto Swapchain::present(const VkQueue& queue, const Semaphore& render_finished_semaphore) noexcept -> Swapchain::Result {

			const auto present_info = VkPresentInfoKHR{.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
				.waitSemaphoreCount = 1,
				.pWaitSemaphores    = &render_finished_semaphore.handle,

				.swapchainCount     = 1,
				.pSwapchains        = &this->handle,

				.pImageIndices      = &this->image_index,
			};


			const VkResult result = vkQueuePresentKHR(queue, &present_info);

			if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR){
				return Swapchain::Result::NeedResize;

			}else if(vulkan::result_fail(result, "vkQueuePresentKHR")){
				PH_FATAL("Failed to present swapchain image");
				return Swapchain::Result::Error;
			}


			return Swapchain::Result::Success;
		};



		
		
	};
};