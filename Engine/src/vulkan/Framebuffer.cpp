#include "framebuffer.h"

#include "Logging.h"

#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/RenderPass.h"

namespace ph{
	namespace vulkan{


		auto Framebuffer::create(
			const Device& device, const RenderPass& renderpass, uint32_t width, uint32_t height, const evo::ArrayProxy<VkImageView> attachments
		) noexcept -> bool {
			PH_ASSERT(this->is_valid() == false, "Framebuffer was already created");



			const auto create_info = VkFramebufferCreateInfo{ .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass      = renderpass.handle,
				.attachmentCount = uint32_t(attachments.size()),
				.pAttachments    = attachments.data(),
				.width           = width,
				.height          = height,
				.layers          = 1,
			};


			const VkResult result = vkCreateFramebuffer(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateFramebuffer")) return false;


			PH_TRACE("Created: Vulkan Framebuffer");
			return true;	
		};



		auto Framebuffer::destroy(const Device& device) noexcept -> void {
			PH_ASSERT(this->is_valid(), "Framebuffer was not created");

			vkDestroyFramebuffer(device.get_handle(), this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;

			PH_TRACE("Destroyed: Vulkan Framebuffer");
		};
		
		
	};
};