#include "synchronization.h"

#include "Logging.h"

#include "vulkan/utils.h"
#include "vulkan/context.h"

namespace ph{
	namespace vulkan{

		//////////////////////////////////////////////////////////////////////
		// fence

		auto Fence::create(const Device& device) noexcept -> bool {
			const auto create_info = VkFenceCreateInfo{ .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
				.flags = VK_FENCE_CREATE_SIGNALED_BIT,
			};

			const VkResult result = vkCreateFence(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateFence")) return false;

			PH_DEBUG("Created: Vulkan fence");
			return true;
		};


		auto Fence::destroy(const Device& device) noexcept -> void {
			vkDestroyFence(device.get_handle(), this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;

			PH_DEBUG("Destroyed: Vulkan fence");
		};


		auto Fence::wait(const Device& device, uint64_t timeout_ns) const noexcept -> bool {
			const VkResult result = vkWaitForFences(device.get_handle(), 1, &this->handle, VK_TRUE, timeout_ns);
			if(vulkan::result_fail(result, "vkWaitForFences")) return false;

			return true;
		};


		auto Fence::reset(const Device& device) const noexcept -> bool {
			if(vkResetFences(device.get_handle(), 1, &this->handle) != VK_SUCCESS){
				PH_ERROR("Failed to reset Vulkan fence");
				return false;
			}

			return true;
		};





		//////////////////////////////////////////////////////////////////////
		// semaphore

		auto Semaphore::create(const Device& device) noexcept -> bool {
			const auto create_info = VkSemaphoreCreateInfo{ .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, };

			const VkResult result = vkCreateSemaphore(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateSemaphore")) return false;

			PH_DEBUG("Created: Vulkan semaphore");
			return true;
		};


		auto Semaphore::destroy(const Device& device) noexcept -> void {
			vkDestroySemaphore(device.get_handle(), this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;

			PH_DEBUG("Destroyed: Vulkan semaphore");
		};
		
		
	};
};