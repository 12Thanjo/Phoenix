#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


namespace ph{
	namespace vulkan{
		
		EVO_NODISCARD auto create_command_pool(const class Device& device, uint32_t queue_family_index) noexcept -> std::optional<VkCommandPool>;
		auto destroy_command_pool(const class Device& device, VkCommandPool& command_pool) noexcept -> void;


		class CommandBuffer{
			public:
				CommandBuffer() = default;
				~CommandBuffer() = default;

				EVO_NODISCARD auto allocate(const class Device& device, const VkCommandPool& command_pool) noexcept -> bool;
				auto free(const class Device& device, const VkCommandPool& command_pool) noexcept -> void;


				EVO_NODISCARD auto begin(bool is_single_use, bool is_renderpass_continue, bool is_simultaneous_use) const noexcept -> bool;
				EVO_NODISCARD auto end() const noexcept -> bool;


				EVO_NODISCARD auto allocate_and_begin_single_use(const class Device& device, const VkCommandPool& command_pool) noexcept -> bool;
				EVO_NODISCARD auto end_and_free_single_use(
					const class Device& device, const VkCommandPool& command_pool, const VkQueue& queue
				) noexcept -> bool;


				///////////////////////////////////
				// vulkan commands

				auto reset() const noexcept -> void;

				auto begin_render_pass(const VkRenderPassBeginInfo& begin_info) const noexcept -> void;
				auto end_render_pass() const noexcept -> void;


				auto set_viewport(const VkViewport& viewport) const noexcept -> void;
				auto set_scissor(const VkRect2D& scissor) const noexcept -> void;


				auto bind_pipeline(const VkPipeline& pipeline, VkPipelineBindPoint bind_point) const noexcept -> void;


				// TODO: bind set(s)?
				auto bind_descriptor_set(
					const VkPipelineLayout& pipeline_layout,
					uint32_t set_index,
					const VkDescriptorSet& descriptor_set,
					VkPipelineBindPoint bind_point
				) const noexcept -> void;



				// TODO: bind vertex buffer(s)?
				auto bind_vertex_buffer(const class Buffer& buffer, uint64_t offset) const noexcept -> void;
				auto bind_index_buffer(const class Buffer& buffer, uint64_t offset) const noexcept -> void;

				// TODO: other parameters
				auto draw_indexed(uint32_t index_count, uint32_t first_index, int32_t vertex_offset) const noexcept -> void;


				auto copy_buffer(
					const class Buffer& src, uint64_t src_offset, class Buffer& dst, uint64_t dst_offset, size_t size
				) const noexcept -> void;
				auto copy_buffer_to_image(const class Buffer& buffer, class Image& image, const VkBufferImageCopy& region) const noexcept -> void;

				// TODO: push constant(s)?
				auto push_constant(
					const VkPipelineLayout& pipeline_layout, VkShaderStageFlags stage_flags, uint32_t size, const void* data
				) const noexcept -> void;

		
			// TODO: private
			public:
				VkCommandBuffer handle{VK_NULL_HANDLE};
		};


	};
};