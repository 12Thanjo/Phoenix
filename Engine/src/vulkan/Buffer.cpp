#include "Buffer.h"

#include "Logging.h"


#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/commands.h"


#include <format>


namespace ph{
	namespace vulkan{

		auto Buffer::create(
			const Device& device,
			size_t size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			bool map_memory
		) noexcept -> bool {


			///////////////////////////////////
			// buffer

			const auto create_info = VkBufferCreateInfo{ .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
				.size        = static_cast<VkDeviceSize>(size),
				.usage       = usage,
				.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			};


			VkResult result = vkCreateBuffer(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateBuffer")) return false;


			///////////////////////////////////
			// memory

			VkMemoryRequirements memory_requirements;
			vkGetBufferMemoryRequirements(device.get_handle(), this->handle, &memory_requirements);

			const auto memory_type_result = vulkan::find_memory_type(device.get_physical(), memory_requirements.memoryTypeBits, properties);
			if(memory_type_result.has_value() == false) return false;


			const auto alloc_info = VkMemoryAllocateInfo{ .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize  = memory_requirements.size,
				.memoryTypeIndex = *memory_type_result,
			};


			result = vkAllocateMemory(device.get_handle(), &alloc_info, nullptr, &this->memory);
			if(vulkan::result_fail(result, "vkAllocateMemory")) return false;

			vkBindBufferMemory(device.get_handle(), this->handle, this->memory, 0);


			///////////////////////////////////
			// memory data

			if(map_memory){
				this->data_size = size;
				vkMapMemory(device.get_handle(), this->memory, 0, this->data_size, 0, &this->data_ptr);
			}



			///////////////////////////////////
			// done

			PH_TRACE("Created: Vulkan Buffer");
			return true;
		};


		auto Buffer::destroy(const Device& device) noexcept -> void {
			if(this->data_ptr != nullptr){
				vkUnmapMemory(device.get_handle(), this->memory);
				this->data_size = 0;
				this->data_ptr = nullptr;
			}


			vkFreeMemory(device.get_handle(), this->memory, nullptr);
			vkDestroyBuffer(device.get_handle(), this->handle, nullptr);


			PH_TRACE("Destroyed: Vulkan buffer");
		};





		auto Buffer::set_data(const void* target_data, size_t offset, size_t target_data_size) noexcept -> void {
			PH_ASSERT(this->handle != VK_NULL_HANDLE, "Buffer was not created yet");
			PH_ASSERT(this->data_ptr != nullptr, "Buffer memory is not mapped");			
			PH_ASSERT(offset < this->data_size, "Offset is outsize buffer range");
			PH_ASSERT(target_data_size + offset <= this->data_size, "Data with offset cannot fit in buffer");

			std::memcpy((byte*)this->data_ptr + offset, target_data, target_data_size);
		};



		auto Buffer::copy_from(
			const class Device& device,
			const VkCommandPool& command_pool,
			const VkQueue& queue,
			const Buffer& src,
			size_t size,
			size_t src_offset,
			size_t dst_offset
		) noexcept -> bool {
			auto command_buffer = vulkan::CommandBuffer{};
			if(command_buffer.allocate_and_begin_single_use(device, command_pool) == false) return false;

				command_buffer.copy_buffer(src, src_offset, *this, dst_offset, size);

			if(command_buffer.end_and_free_single_use(device, command_pool, queue) == false) return false;

			PH_TRACE(std::format("Copied data from one Vulkan buffer to another ({} B)", size));
			return true;
		};
		
		
	};
};