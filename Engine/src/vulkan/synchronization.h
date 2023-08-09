#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


namespace ph{
	namespace vulkan{
		
		class Fence{
			public:
				Fence() = default;
				~Fence() = default;

				EVO_NODISCARD auto create(const class Device& device) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;


				EVO_NODISCARD auto wait(const class Device& device, uint64_t timeout_ns = UINT64_MAX) const noexcept -> bool;
				EVO_NODISCARD auto reset(const class Device& device) const noexcept -> bool;

		
			// TODO: private
			public:
				VkFence handle{VK_NULL_HANDLE};
		};



		class Semaphore{
			public:
				Semaphore() = default;
				~Semaphore() = default;

				EVO_NODISCARD auto create(const class Device& device) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;
		
			// TODO: private
			public:
				VkSemaphore handle{VK_NULL_HANDLE};
		};


	};
};