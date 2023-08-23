#include "RenderPass.h"

#include "Logging.h"

#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/commands.h"
#include "vulkan/Buffer.h"
#include "vulkan/images.h"


namespace ph{
	namespace vulkan{


		auto RenderPass::create(
			const Device& device, VkSampleCountFlagBits msaa_samples, bool has_prev, bool has_next, const evo::ArrayProxy<Attachment> attachment_list
		) noexcept -> bool {

			///////////////////////////////////
			// attachments

			auto attachment_descriptions = std::vector<VkAttachmentDescription>{};

			auto color_attachment_refs = std::vector<VkAttachmentReference>{};
			auto depth_attachment_ref = std::optional<VkAttachmentReference>{};
			auto resolve_attachment_ref = std::optional<VkAttachmentReference>{};


			bool has_resolve = false;
			for(const auto& attachment : attachment_list){
				if(attachment.type == AttachmentType::Resolve){
					has_resolve = true;
					break;
				}
			}



			for(uint32_t i = 0; i < attachment_list.size(); i+=1){
				const auto& attachment = attachment_list[i];

				switch(attachment.type){
					case AttachmentType::Color: {

						attachment_descriptions.emplace_back() = VkAttachmentDescription{
							.format         = attachment.format,
							.samples        = msaa_samples,

							.loadOp         = attachment.should_clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
							.storeOp        = VK_ATTACHMENT_STORE_OP_STORE,

							.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
							.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

							.initialLayout  =  has_prev                 ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_UNDEFINED,
							.finalLayout    = (has_next || has_resolve) ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
						};

						color_attachment_refs.emplace_back() = VkAttachmentReference{
							.attachment = i,
							.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
						};

						if(attachment.should_clear){
							this->clear_values.emplace_back() = VkClearValue{
								.color = {attachment.clear.color.r, attachment.clear.color.g, attachment.clear.color.b, attachment.clear.color.a}
							};
						}

					} break;


					case AttachmentType::DepthStencil: {
						
						attachment_descriptions.emplace_back() = VkAttachmentDescription{
							.format         = attachment.format,
							.samples        = msaa_samples,

							.loadOp         = attachment.should_clear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,
							.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,

							.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
							.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

							.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
							.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
						};

						depth_attachment_ref = VkAttachmentReference{
							.attachment = i,
							.layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
						};

						if(attachment.should_clear){
							this->clear_values.emplace_back() = VkClearValue{
								.depthStencil = {
									.depth   = attachment.clear.depth_stencil.depth,
									.stencil = attachment.clear.depth_stencil.stencil,
								},
							};
						}

					} break;

					case AttachmentType::Resolve: {
						
						attachment_descriptions.emplace_back() = VkAttachmentDescription{
							.format         = attachment.format,
							.samples        = VK_SAMPLE_COUNT_1_BIT,

							.loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
							.storeOp        = VK_ATTACHMENT_STORE_OP_STORE,

							.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
							.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

							.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
							.finalLayout    = has_next ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
						};

						resolve_attachment_ref = VkAttachmentReference{
							.attachment = i,
							.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
						};


						PH_ASSERT(attachment.should_clear == false, "Resolve attachments should not clear");

					} break;
				};
			}




			///////////////////////////////////
			// subpass

			// main subpass
			const auto subpass = VkSubpassDescription{
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,

				// input from a shader
				.inputAttachmentCount = 0,
				.pInputAttachments = nullptr,

				// color attachment
				.colorAttachmentCount = uint32_t(color_attachment_refs.size()),
				.pColorAttachments = color_attachment_refs.data(),

				// multisampling color attachments
				.pResolveAttachments = resolve_attachment_ref.has_value() ? &*resolve_attachment_ref : nullptr,

				// depth stencil data
				// .pDepthStencilAttachment = (depth_attachment_reference.attachment != ~0ul) ? &depth_attachment_reference : nullptr,
				.pDepthStencilAttachment = depth_attachment_ref.has_value() ? &*depth_attachment_ref : nullptr,

				// preserved for next
				.preserveAttachmentCount = 0,
				.pPreserveAttachments = nullptr,
			};


			///////////////////////////////////
			// dependency

			// TODO: is this all correct?
			const auto dependency = VkSubpassDependency{
				.srcSubpass = VK_SUBPASS_EXTERNAL,
				.dstSubpass = 0,

				.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
				.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,

				.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
				.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			};


			///////////////////////////////////
			// create

			// std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve };


			const auto create_info = VkRenderPassCreateInfo{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
				.attachmentCount = uint32_t(attachment_descriptions.size()),
				.pAttachments = attachment_descriptions.data(),

				.subpassCount = 1,
				.pSubpasses = &subpass,

				.dependencyCount = 1,
				.pDependencies = &dependency,
			};

			const VkResult result = vkCreateRenderPass(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateRenderPass")) return false;

			PH_DEBUG("Created: Vulkan render pass");
			return true;
		};



		auto RenderPass::destroy(const Device& device) noexcept -> void {
			vkDestroyRenderPass(device.get_handle(), this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;

			PH_DEBUG("Destroyed: Vulkan render pass");
		};




		auto RenderPass::begin(
			const CommandBuffer& command_buffer, const Framebuffer& framebuffer, uint32_t width, uint32_t height
		) noexcept -> void {


			const auto pass_begin_info = VkRenderPassBeginInfo{ .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
				.renderPass      = this->handle,
				.framebuffer     = framebuffer.handle,
				.renderArea      = {
					.offset = {0, 0},
					.extent = {width, height},
				},
				.clearValueCount = uint32_t(this->clear_values.size()),
				.pClearValues    = this->clear_values.data(),
			};



			command_buffer.begin_render_pass(pass_begin_info);

		};




		auto RenderPass::end(const CommandBuffer& command_buffer) noexcept -> void {
			command_buffer.end_render_pass();
		};




		//////////////////////////////////////////////////////////////////////
		// render pass manager


		auto RenderPassManager::create(
			const Device& device,
			VkSampleCountFlagBits msaa_samples,
			bool has_prev,
			bool has_next,
			const evo::ArrayProxy<Attachment> attachment_list,
			const evo::ArrayProxy<VkPushConstantRange> push_constant_ranges
		) noexcept -> bool {

			if(this->render_pass.create(device, msaa_samples, has_prev, has_next, attachment_list) == false){
				PH_ERROR("Render pass manager failed to create vulkan render pass");
				return false;
			}


			const auto pipeline_layout_result = vulkan::create_pipeline_layout(device, this->descriptor_set_layouts, push_constant_ranges);
			if(pipeline_layout_result.has_value() == false){
				PH_ERROR("Render pass manager failed to create pipeline layout");
				return false;
			}

			this->pipeline_layout = std::move(*pipeline_layout_result);



			PH_DEBUG("Created: Vulkan render pass manager");
			return true;
		};




		auto RenderPassManager::destroy(const Device& device) noexcept -> void {
			for(auto& layout_info : this->descriptor_set_layout_infos){
				for(auto& descriptor_pool : layout_info.descriptor_pools){
					vulkan::destroy_descriptor_pool(device, descriptor_pool);
				}
			}
			evo::vector_dealloc(this->descriptor_set_layout_infos);


			for(auto& descriptor_set_layout : this->descriptor_set_layouts){
				vulkan::destroy_descriptor_set_layout(device, descriptor_set_layout);
			}
			evo::vector_dealloc(this->descriptor_set_layouts);


			for(auto& pipeline : this->pipelines){
				pipeline.destroy(device);
			}
			evo::vector_dealloc(this->pipelines);


			vulkan::destroy_pipeline_layout(device, this->pipeline_layout);

			this->render_pass.destroy(device);

			PH_DEBUG("Destroyed: Vulkan render pass manager");
		};


		auto RenderPassManager::begin(
			const CommandBuffer& command_buffer, const Framebuffer& framebuffer, uint32_t width, uint32_t height
		) noexcept -> void {
			this->render_pass.begin(command_buffer, framebuffer, width, height);
		};

		auto RenderPassManager::end(const CommandBuffer& command_buffer) noexcept -> void {
			this->render_pass.end(command_buffer);
		};




		auto RenderPassManager::add_descriptor_set_layout(
			const Device& device, uint32_t pool_allocation_size, const evo::ArrayProxy<VkDescriptorSetLayoutBinding> bindings
		) noexcept -> std::optional<DescriptorSetLayoutID> {
			
			const auto result = vulkan::create_descriptor_set_layout(device, bindings);
			if(result.has_value() == false) return std::nullopt;

			this->descriptor_set_layouts.emplace_back(*result);
			this->descriptor_set_layout_infos.emplace_back() = DescriptorSetLayoutInfo{
				.bindings = std::vector(bindings.front(), bindings.back()),
				.pool_allocation_size = pool_allocation_size
			};

			const auto layout_id = DescriptorSetLayoutID{ uint32_t(this->descriptor_set_layouts.size() - 1) };
			this->allocate_descriptor_pool(device, layout_id);

			return layout_id;
		};



		auto RenderPassManager::create_pipeline(
			const class Device& device, const PipelineConfig& config, const VkPipelineCache& pipeline_cache
		) noexcept -> std::optional<PipelineID> {

			auto new_pipeline = Pipeline{};

			if(new_pipeline.create(device, this->render_pass, this->pipeline_layout, config, pipeline_cache) == false){
				PH_ERROR("Failed to create Vulkan pipeline");
				return std::nullopt;
			}


			this->pipelines.emplace_back(std::move(new_pipeline));

			return PipelineID{ uint32_t(this->pipelines.size() - 1) };
		};



		// TODO: allocate new pool if necessary
		auto RenderPassManager::allocate_descriptor_set(
			const Device& device, DescriptorSetLayoutID layout_id
		) noexcept -> std::optional<DescriptorSetID> {
			auto& layout_info = this->descriptor_set_layout_infos[layout_id.id];

			// allocate another pool if necessary
			if(layout_info.descriptor_sets.size() >= layout_info.descriptor_pools.size() * layout_info.pool_allocation_size){
				this->allocate_descriptor_pool(device, layout_id);
			}


			auto result = vulkan::allocate_descriptor_sets(device, layout_info.descriptor_pools.back(), this->descriptor_set_layouts[layout_id.id]);

			if(result.has_value() == false){
				PH_FATAL("Failed to allocate descriptor set");
				return std::nullopt;
			}

			layout_info.descriptor_sets.emplace_back(result->operator[](0));

			return DescriptorSetID{ uint32_t(layout_info.descriptor_sets.size() - 1) };
		};



		auto RenderPassManager::write_descriptor_set_ubo(
			const Device& device, DescriptorSetLayoutID layout_id, DescriptorSetID set_id, uint32_t binding, const Buffer& buffer
		) noexcept -> void {

			const auto buffer_info = VkDescriptorBufferInfo{
				.buffer = buffer.handle,
				.offset = 0,
				.range  = buffer.size(),
			};

			
			const auto descriptor_writes = std::array<VkWriteDescriptorSet, 1>{
				VkWriteDescriptorSet{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet          = this->descriptor_set_layout_infos[layout_id.id].descriptor_sets[set_id.id],
					.dstBinding      = binding,
					.dstArrayElement = 0,

					.descriptorCount = 1,
					.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
					.pBufferInfo     = &buffer_info,
				},
			};


			vkUpdateDescriptorSets(
				device.get_handle(), uint32_t(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr
			);
		};




		auto RenderPassManager::write_descriptor_set_texture(
			const Device& device, DescriptorSetLayoutID layout_id, DescriptorSetID set_id, uint32_t binding, const Texture& texture
		) noexcept -> void {

			const auto image_info = VkDescriptorImageInfo{
				.sampler     = texture.get_sampler(),
				.imageView   = texture.get_image_view(),
				.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			};

			
			const auto descriptor_writes = std::array<VkWriteDescriptorSet, 1>{
				VkWriteDescriptorSet{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet          = this->descriptor_set_layout_infos[layout_id.id].descriptor_sets[set_id.id],
					.dstBinding      = binding,
					.dstArrayElement = 0,

					.descriptorCount = 1,
					.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.pImageInfo      = &image_info,
				},
			};


			vkUpdateDescriptorSets(
				device.get_handle(), uint32_t(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr
			);

		};




		auto RenderPassManager::bind_descriptor_set(
			const CommandBuffer& command_buffer, DescriptorSetLayoutID layout_id, DescriptorSetID set_id
		) noexcept -> void {
			command_buffer.bind_descriptor_set(
				this->pipeline_layout, layout_id.id, this->descriptor_set_layout_infos[layout_id.id].descriptor_sets[set_id.id], VK_PIPELINE_BIND_POINT_GRAPHICS
			);
		};


		auto RenderPassManager::bind_graphics_pipeline(const CommandBuffer& command_buffer, PipelineID id) noexcept -> void {
			command_buffer.bind_pipeline(this->pipelines[id.id].handle, VK_PIPELINE_BIND_POINT_GRAPHICS);
		};


		auto RenderPassManager::set_push_constant(
			const CommandBuffer& command_buffer, VkShaderStageFlags stage_flags, uint32_t size, const void* data
		) noexcept -> void {
			command_buffer.push_constant(this->pipeline_layout, stage_flags, size, data);
		};





		auto RenderPassManager::allocate_descriptor_pool(const Device& device, DescriptorSetLayoutID layout_id) noexcept -> bool {
			DescriptorSetLayoutInfo& info = this->descriptor_set_layout_infos[layout_id.id];

			auto pool_sizes = std::vector<VkDescriptorPoolSize>{};

			for(const auto& binding : info.bindings){
				pool_sizes.emplace_back(binding.descriptorType, binding.descriptorCount * info.pool_allocation_size);
			}


			const auto descriptor_pool_result = vulkan::create_descriptor_pool(device, info.pool_allocation_size, pool_sizes);

			if(descriptor_pool_result.has_value() == false){
				PH_FATAL("Failed to create descriptor pool");
				return false;
			}

			info.descriptor_pools.emplace_back(std::move(*descriptor_pool_result));

			return true;
		};



		
		
	};
};