#pragma once

#include <libs/Vulkan/Vulkan.h>
#include <libs/glm/glm.h>
#include <libs/evo/evo.h>


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


	};
};