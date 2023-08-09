#include "RenderPass.h"

#include "Logging.h"

#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/commands.h"

namespace ph{
	namespace vulkan{


		auto RenderPass::create(
			const class Device& device,
			VkSampleCountFlagBits msaa_samples,
			bool has_prev,
			bool has_next,
			const evo::ArrayProxy<Attachment> attachment_list
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
				.colorAttachmentCount = static_cast<uint32_t>(color_attachment_refs.size()),
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
				.attachmentCount = static_cast<uint32_t>(attachment_descriptions.size()),
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
				.clearValueCount = static_cast<uint32_t>(this->clear_values.size()),
				.pClearValues    = this->clear_values.data(),
			};



			command_buffer.begin_render_pass(pass_begin_info);

		};




		auto RenderPass::end(const CommandBuffer& command_buffer) noexcept -> void {
			command_buffer.end_render_pass();
		};





		
		
	};
};