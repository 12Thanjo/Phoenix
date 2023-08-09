#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


namespace ph{
	namespace vulkan{
		
		
		class Buffer{
			public:
				Buffer() = default;
				~Buffer() = default;

				EVO_NODISCARD auto create(
					const class Device& device,
					size_t size,
					VkBufferUsageFlags usage,
					VkMemoryPropertyFlags properties,
					bool map_memory
				) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;


				auto set_data(const void* target_data, size_t offset, size_t target_data_size) noexcept -> void;

				inline auto set_data(const void* target_data) noexcept -> void { this->set_data(target_data, 0, this->data_size); };


				EVO_NODISCARD auto copy_from(
					const class Device& device,
					const VkCommandPool& command_pool,
					const VkQueue& queue,
					const Buffer& src,
					size_t size,
					size_t src_offset,
					size_t dst_offset
				) noexcept -> bool;


				EVO_NODISCARD constexpr auto data() const noexcept -> void*  { return this->data_ptr;  };
				EVO_NODISCARD constexpr auto size() const noexcept -> size_t { return this->data_size; };
		
			public:
				VkBuffer handle{VK_NULL_HANDLE};
				VkDeviceMemory memory{VK_NULL_HANDLE};

			private:
				void* data_ptr = nullptr;
				size_t data_size = 0;

		};

	};
};