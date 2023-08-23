#pragma once

#include <libs/Vulkan/Vulkan.h>
#include <libs/glm/glm.h>
#include <libs/evo/evo.h>

#include "vulkan/Pipeline.h"
#include "vulkan/utils.h"

namespace ph{
	namespace vulkan{


		enum class AttachmentType{
			Color,
			DepthStencil,
			Resolve,
		};


		struct Attachment{
			AttachmentType type;
			bool should_clear = true;
			VkFormat format;

			union Clear{
				glm::vec4 color;

				struct {
					float depth;
					uint32_t stencil;
				} depth_stencil;



				Clear(glm::vec4 color_val)
					: color(color_val) {};

				Clear(float depth_val, uint32_t stencil_val)
					: depth_stencil(depth_val, stencil_val) {};


				Clear() = default;
			} clear;



			Attachment(AttachmentType _type, VkFormat image_format, glm::vec4 clear_color)
				: type(_type), format(image_format), clear(clear_color) {};

			Attachment(AttachmentType _type, VkFormat image_format, float depth_val, uint32_t stencil_val)
				: type(_type), format(image_format), clear(depth_val, stencil_val) {};

			Attachment(AttachmentType _type, VkFormat image_format)
				: type(_type), format(image_format), should_clear(false) {};
		};



		
		class RenderPass{
			public:
				RenderPass() = default;
				~RenderPass() = default;


				EVO_NODISCARD auto create(
					const class Device& device,
					VkSampleCountFlagBits msaa_samples,
					bool has_prev,
					bool has_next,
					const evo::ArrayProxy<Attachment> attachment_list
				) noexcept -> bool;


				auto destroy(const class Device& device) noexcept -> void;


				auto begin(
					const class CommandBuffer& command_buffer, const class Framebuffer& framebuffer, uint32_t width, uint32_t height
				) noexcept -> void;

				auto end(const class CommandBuffer& command_buffer) noexcept -> void;
		
			// TODO: private
			public:
				VkRenderPass handle{VK_NULL_HANDLE};

			private:
				evo::StaticVector<VkClearValue, 4> clear_values{};
		};



		//////////////////////////////////////////////////////////////////////
		// render pass manager

		class RenderPassManager{
			public:
				RenderPassManager() = default;
				~RenderPassManager() = default;

				///////////////////////////////////
				// these functions must be called before RenderPassManager::create()

				struct DescriptorSetLayoutID{ uint32_t id; };

				// `pool_allocation_size` is the number of descriptor sets per pool created (will create more pools if needed)
				EVO_NODISCARD auto add_descriptor_set_layout(
					const class Device& device, uint32_t pool_allocation_size, const evo::ArrayProxy<VkDescriptorSetLayoutBinding> bindings
				) noexcept -> std::optional<DescriptorSetLayoutID>;



				// these functions must be called before RenderPassManager::create()
				///////////////////////////////////



				EVO_NODISCARD auto create(
					const class Device& device,
					VkSampleCountFlagBits msaa_samples,
					bool has_prev,
					bool has_next,
					const evo::ArrayProxy<Attachment> attachment_list,
					const evo::ArrayProxy<VkPushConstantRange> push_constant_ranges
				) noexcept -> bool;

				auto destroy(const class Device& device) noexcept -> void;


				auto begin(
					const class CommandBuffer& command_buffer, const class Framebuffer& framebuffer, uint32_t width, uint32_t height
				) noexcept -> void;

				auto end(const class CommandBuffer& command_buffer) noexcept -> void;
				


				struct PipelineID{ uint32_t id; };
				EVO_NODISCARD auto create_pipeline(
					const class Device& device, const PipelineConfig& config, const VkPipelineCache& pipeline_cache = VK_NULL_HANDLE
				) noexcept -> std::optional<PipelineID>;


				struct DescriptorSetID{ uint32_t id; };
				EVO_NODISCARD auto allocate_descriptor_set(
					const class Device& device, DescriptorSetLayoutID layout_id
				) noexcept -> std::optional<DescriptorSetID>;



				auto write_descriptor_set_ubo(
					const class Device& device, DescriptorSetLayoutID layout_id, DescriptorSetID set_id, uint32_t binding, const class Buffer& buffer
				) noexcept -> void;

				auto write_descriptor_set_texture(
					const class Device& device, DescriptorSetLayoutID layout_id, DescriptorSetID set_id, uint32_t binding, const class Texture& texture
				) noexcept -> void;


				auto bind_descriptor_set(
					const class CommandBuffer& command_buffer, DescriptorSetLayoutID layout_id, DescriptorSetID set_id
				) noexcept -> void;



				auto bind_graphics_pipeline(const class CommandBuffer& command_buffer, PipelineID id) noexcept -> void;


				auto set_push_constant(
					const class CommandBuffer& command_buffer, VkShaderStageFlags stage_flags, uint32_t size, const void* data
				) noexcept -> void;


				inline auto get_render_pass() const noexcept -> const RenderPass& { return this->render_pass; };
				

			private:
				EVO_NODISCARD auto allocate_descriptor_pool(const class Device& device, DescriptorSetLayoutID layout_id) noexcept -> bool;
			

			private:
				RenderPass render_pass{};
				VkPipelineLayout pipeline_layout{VK_NULL_HANDLE};
				std::vector<Pipeline> pipelines{};



				struct DescriptorSetLayoutInfo{
					std::vector<VkDescriptorPool> descriptor_pools{};
					std::vector<VkDescriptorSetLayoutBinding> bindings{};
					std::vector<VkDescriptorSet> descriptor_sets{};
					uint32_t pool_allocation_size;
				};

				// there should be one std::vector<DescriptorSetLayoutInfo> for every descriptor set layout
				std::vector<DescriptorSetLayoutInfo> descriptor_set_layout_infos{};

				std::vector<VkDescriptorSetLayout> descriptor_set_layouts{};
		};


	};
};