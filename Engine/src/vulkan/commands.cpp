#include "commands.h"

#include "Logging.h"


#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/Buffer.h"
#include "vulkan/images.h"


namespace ph{
	namespace vulkan{

		auto create_command_pool(const Device& device, uint32_t queue_family_index) noexcept -> std::optional<VkCommandPool> {
			const auto create_info = VkCommandPoolCreateInfo{ .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
				.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
				.queueFamilyIndex = queue_family_index,
			};

			VkCommandPool command_pool;
			const VkResult result = vkCreateCommandPool(device.get_handle(), &create_info, nullptr, &command_pool);
			if(vulkan::result_fail(result, "vkCreateCommandPool")) return std::nullopt;

			PH_DEBUG("Created: Vulkan command pool");
			return std::move(command_pool);
		};


		auto destroy_command_pool(const Device& device, VkCommandPool& command_pool) noexcept -> void {
			vkDestroyCommandPool(device.get_handle(), command_pool, nullptr);
			PH_DEBUG("Destroyed: Vulkan command pool");
		};



		//////////////////////////////////////////////////////////////////////
		// command buffer

		auto CommandBuffer::allocate(const Device& device, const VkCommandPool& command_pool) noexcept -> bool {
			const auto alloc_info = VkCommandBufferAllocateInfo{ .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
				.commandPool        = command_pool,
				.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
				.commandBufferCount = 1,
			};


			const VkResult result = vkAllocateCommandBuffers(device.get_handle(), &alloc_info, &this->handle);
			if(vulkan::result_fail(result, "vkAllocateCommandBuffers")) return false;

			PH_TRACE("Allocated Vulkan command buffer");
			return true;
		};




		auto CommandBuffer::free(const Device& device, const VkCommandPool& command_pool) noexcept -> void {
			vkFreeCommandBuffers(device.get_handle(), command_pool, 1, &this->handle);
			this->handle = VK_NULL_HANDLE;

			PH_TRACE("Freed Vulkan command buffer");
		};



		auto CommandBuffer::begin(bool is_single_use, bool is_renderpass_continue, bool is_simultaneous_use) const noexcept -> bool {
			auto begin_info = VkCommandBufferBeginInfo{
				.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				.flags = 0,
			};

			if(is_single_use)          begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			if(is_renderpass_continue) begin_info.flags |= VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
			if(is_simultaneous_use)    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;


			const VkResult result = vkBeginCommandBuffer(this->handle, &begin_info);
			if(vulkan::result_fail(result, "vkBeginCommandBuffer")) return false;

			return true;
		};


		auto CommandBuffer::end() const noexcept -> bool {
			const VkResult result = vkEndCommandBuffer(this->handle);
			if(vulkan::result_fail(result, "vkEndCommandBuffer")) return false;

			return true;
		};




		auto CommandBuffer::allocate_and_begin_single_use(const Device& device, const VkCommandPool& command_pool) noexcept -> bool {
			if(this->allocate(device, command_pool) == false) return false;
			if(this->begin(true, false, false) == false) return false;

			PH_TRACE("Began single use Vulkan buffer");
			return true;
		};


		auto CommandBuffer::end_and_free_single_use(const Device& device, const VkCommandPool& command_pool, const VkQueue& queue) noexcept -> bool {
			if(this->end() == false) return false;

			const auto submit_info = VkSubmitInfo{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
				.commandBufferCount = 1,
				.pCommandBuffers    = &this->handle,
			};


			const VkResult result = vkQueueSubmit(queue, 1, &submit_info, VK_NULL_HANDLE);
			if(vulkan::result_fail(result, "vkQueueSubmit")) return false;


			if(vulkan::result_fail(vkQueueWaitIdle(queue), "vkQueueWaitIdle")) return false;

			
			this->free(device, command_pool);

			PH_TRACE("Ended single use Vulkan buffer");
			return true;
		};






		//////////////////////////////////////////////////////////////////////
		// vulkan commands

		auto CommandBuffer::reset() const noexcept -> void {
			constexpr VkCommandBufferResetFlagBits flags = (VkCommandBufferResetFlagBits)0;
			vkResetCommandBuffer(this->handle, flags);
		};



		auto CommandBuffer::begin_render_pass(const VkRenderPassBeginInfo& begin_info) const noexcept -> void {
			vkCmdBeginRenderPass(this->handle, &begin_info, VK_SUBPASS_CONTENTS_INLINE);
		};

		auto CommandBuffer::end_render_pass() const noexcept -> void {
			vkCmdEndRenderPass(this->handle);
		};





		auto CommandBuffer::set_viewport(const VkViewport& viewport) const noexcept -> void {
			vkCmdSetViewport(this->handle, 0, 1, &viewport);
		};

		auto CommandBuffer::set_scissor(const VkRect2D& scissor) const noexcept -> void {
			vkCmdSetScissor(this->handle, 0, 1, &scissor);
		};




		auto CommandBuffer::bind_pipeline(const VkPipeline& pipeline, VkPipelineBindPoint bind_point) const noexcept -> void {
			vkCmdBindPipeline(this->handle, bind_point, pipeline);
		};


		auto CommandBuffer::bind_descriptor_set(
			const VkPipelineLayout& pipeline_layout,
			uint32_t set_index,
			const VkDescriptorSet& descriptor_set,
			VkPipelineBindPoint bind_point
		) const noexcept -> void {
			vkCmdBindDescriptorSets(this->handle, bind_point, pipeline_layout, set_index, 1, &descriptor_set, 0, nullptr);
		};


		auto CommandBuffer::bind_vertex_buffer(const Buffer& buffer, uint64_t offset) const noexcept -> void {
			vkCmdBindVertexBuffers(this->handle, 0, 1, &buffer.handle, &offset);
		};

		auto CommandBuffer::bind_index_buffer(const Buffer& buffer, uint64_t offset) const noexcept -> void {
			vkCmdBindIndexBuffer(this->handle, buffer.handle, offset, VK_INDEX_TYPE_UINT32);
		};

		auto CommandBuffer::draw_indexed(uint32_t index_count, uint32_t first_index, int32_t vertex_offset) const noexcept -> void {
			vkCmdDrawIndexed(this->handle, index_count, 1, first_index, vertex_offset, 0);
		};


		auto CommandBuffer::copy_buffer(
			const Buffer& src, uint64_t src_offset, Buffer& dst, uint64_t dst_offset, size_t size
		) const noexcept -> void {
			
			const auto copy_region = VkBufferCopy{
				.srcOffset = src_offset,
				.dstOffset = dst_offset,
				.size      = size,
			};

			vkCmdCopyBuffer(this->handle, src.handle, dst.handle, 1, &copy_region);
		};



		auto CommandBuffer::copy_buffer_to_image(const Buffer& buffer, Image& image, const VkBufferImageCopy& region) const noexcept -> void {
			vkCmdCopyBufferToImage(this->handle, buffer.handle, image.handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		};



		auto CommandBuffer::push_constant(
			const VkPipelineLayout& pipeline_layout, VkShaderStageFlags stage_flags, uint32_t size, const void* data
		) const noexcept -> void {
			vkCmdPushConstants(this->handle, pipeline_layout, stage_flags, 0, size, data);
		};


		
		
	};
};