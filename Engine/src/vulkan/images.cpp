#include "images.h"

#include "Logging.h"

#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/Buffer.h"
#include "vulkan/commands.h"

namespace ph{
	namespace vulkan{


		auto create_image_view(
			const Device& device, const VkImage& image, VkFormat format, VkImageAspectFlags aspect_flags, uint32_t mip_levels
		) noexcept -> std::optional<VkImageView> {

			const auto create_info = VkImageViewCreateInfo{.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
				.image            = image,
				.viewType         = VK_IMAGE_VIEW_TYPE_2D,
				.format           = format,
				.subresourceRange = {
					.aspectMask     = aspect_flags,
					.baseMipLevel   = 0,
					.levelCount     = mip_levels,
					.baseArrayLayer = 0,
					.layerCount     = 1,
				},
			};

			VkImageView image_view;
			const VkResult result = vkCreateImageView(device.get_handle(), &create_info, nullptr, &image_view);
			if(vulkan::result_fail(result, "vkCreateImageView")) return std::nullopt;


			PH_TRACE("Created: Vulkan image view");
			return std::move(image_view);
		};



		auto create_image_sampler(const Device& device, uint32_t mip_levels) noexcept -> std::optional<VkSampler> {

			VkPhysicalDeviceProperties properties;
			vkGetPhysicalDeviceProperties(device.get_physical(), &properties);

			const auto create_info = VkSamplerCreateInfo{ .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
				.magFilter               = VK_FILTER_LINEAR,
				.minFilter               = VK_FILTER_LINEAR,
				.mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
				.addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				.addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				.addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				.mipLodBias              = 0.0f,
				.anisotropyEnable        = VK_TRUE,
				.maxAnisotropy           = properties.limits.maxSamplerAnisotropy,
				.compareEnable           = VK_FALSE,
				.compareOp               = VK_COMPARE_OP_ALWAYS,
				.minLod                  = 0.0f,
				.maxLod                  = float(mip_levels),
				.borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
				.unnormalizedCoordinates = VK_FALSE,
			};

			VkSampler sampler;
			const VkResult result = vkCreateSampler(device.get_handle(), &create_info, nullptr, &sampler);
			if(vulkan::result_fail(result, "vkCreateSampler")) return std::nullopt;


			PH_TRACE("Created: Vulkan image sampler");
			return std::move(sampler);
		};

		auto destroy_image_sampler(const Device& device, VkSampler& sampler) noexcept -> void {
			vkDestroySampler(device.get_handle(), sampler, nullptr);
			sampler = VK_NULL_HANDLE;
			
			PH_TRACE("Destroyed: Vulkan image sampler");
		};




		//////////////////////////////////////////////////////////////////////
		// image

		auto Image::create(
			const Device& device,
			VkMemoryPropertyFlags properties,
			uint32_t image_width,
			uint32_t image_height,
			VkFormat format,
			VkImageUsageFlags usage,
			uint32_t image_mip_levels,
			VkSampleCountFlagBits num_samples,
			VkImageTiling tiling,
			VkImageAspectFlags view_aspect_flags
		) noexcept -> bool {

			///////////////////////////////////
			// image

			const auto create_info = VkImageCreateInfo{ .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
				.imageType     = VK_IMAGE_TYPE_2D,
				.format        = format,
				.extent        = {image_width, image_height, 1},
				.mipLevels     = image_mip_levels,
				.arrayLayers   = 1,
				.samples       = num_samples,
				.tiling        = tiling,
				.usage         = usage,
				.sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			};

			VkResult result = vkCreateImage(device.get_handle(), &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateImage")) return false;

			this->width = image_width;
			this->height = image_height;
			this->mip_levels = image_mip_levels;


			///////////////////////////////////
			// memory

			VkMemoryRequirements memory_requirements;
			vkGetImageMemoryRequirements(device.get_handle(), this->handle, &memory_requirements);

			const auto memory_type_result = vulkan::find_memory_type(device.get_physical(), memory_requirements.memoryTypeBits, properties);
			if(memory_type_result.has_value() == false) return false;

			const auto alloc_info = VkMemoryAllocateInfo{ .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				.allocationSize  = memory_requirements.size,
				.memoryTypeIndex = *memory_type_result,
			};


			result = vkAllocateMemory(device.get_handle(), &alloc_info, nullptr, &this->memory);
			if(vulkan::result_fail(result, "vkAllocateMemory")) return false;


			vkBindImageMemory(device.get_handle(), this->handle, this->memory, 0);


			///////////////////////////////////
			// view

			auto image_view = vulkan::create_image_view(device, this->handle, format, view_aspect_flags, this->mip_levels);
			if(image_view.has_value() == false) return false;
			
			this->view = std::move(*image_view);


			///////////////////////////////////
			// done

			PH_DEBUG("Created: Vulkan image");
			return true;
		};


		auto Image::destroy(const Device& device) noexcept -> void {
			vkDestroyImageView(device.get_handle(), this->view, nullptr);
			vkFreeMemory(device.get_handle(), this->memory, nullptr);
			vkDestroyImage(device.get_handle(), this->handle, nullptr);

			PH_DEBUG("Destroyed: Vulkan image");
		};




		auto Image::copy_from_buffer(const Device& device, const VkCommandPool& command_pool, const VkQueue& queue, const Buffer& buffer) noexcept -> bool {
			auto command_buffer = vulkan::CommandBuffer{};
			if(command_buffer.allocate_and_begin_single_use(device, command_pool) == false) return false;


			const auto region = VkBufferImageCopy{
				.bufferOffset      = 0,
				.bufferRowLength   = 0,
				.bufferImageHeight = 0,
				.imageSubresource  = {
					.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
					.mipLevel       = 0,
					.baseArrayLayer = 0,
					.layerCount     = 1,
				},
				.imageOffset = {0, 0, 0},
				.imageExtent = {this->width, this->height, 1},
			};


			command_buffer.copy_buffer_to_image(buffer, *this, region);

			if(command_buffer.end_and_free_single_use(device, command_pool, queue) == false) return false;

			PH_TRACE("Copied image data from Vulkan buffer");
			return true;
		};




		auto Image::transition_layout(
			const Device& device,
			const VkCommandPool& command_pool,
			const VkQueue& queue,
			VkImageLayout old_layout,
			VkImageLayout new_layout
		) noexcept -> bool {
			
			auto command_buffer = vulkan::CommandBuffer{};
			if(command_buffer.allocate_and_begin_single_use(device, command_pool) == false) return false;

			auto barrier = VkImageMemoryBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.oldLayout           = old_layout,
				.newLayout           = new_layout,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image               = this->handle,
				.subresourceRange    = {
					.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
					.baseMipLevel   = 0,
					.levelCount     = this->mip_levels,
					.baseArrayLayer = 0,
					.layerCount     = 1,
				},
			};

			VkPipelineStageFlags src_stage;
			VkPipelineStageFlags dst_stage;

			if(old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL){
				barrier.srcAccessMask = 0;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;

			}else if(old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL){
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			}else{
				PH_FATAL("Invalid, unknown, or unsupported layout transition");
				return false;
			}

			vkCmdPipelineBarrier(
				command_buffer.handle,
				src_stage, dst_stage,
				0,
				0, nullptr,
				0, nullptr,
				1, &barrier
			);

			if(command_buffer.end_and_free_single_use(device, command_pool, queue) == false) return false;

			PH_TRACE("Transitioned Vulkan image layout");
			return true;
		};








		auto Image::generate_mipmaps(
			const Device& device,
			const VkCommandPool& command_pool,
			const VkQueue& queue,
			VkFormat format
		) noexcept -> bool {

			// Check if image format supports linear blitting
			VkFormatProperties format_properties;
			vkGetPhysicalDeviceFormatProperties(device.get_physical(), format, &format_properties);

			if( !(format_properties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) ){
				PH_ERROR("Texture image format does not support linear blitting");
				return false;
			}

			auto command_buffer = vulkan::CommandBuffer{};
			if(command_buffer.allocate_and_begin_single_use(device, command_pool) == false) return false;

			auto barrier = VkImageMemoryBarrier{ .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
				.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
				.image               = this->handle,
				.subresourceRange    = {
					.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
					.levelCount     = 1,
					.baseArrayLayer = 0,
					.layerCount     = 1,
				},
			};

			int32_t mip_width = this->width;
			int32_t mip_height = this->height;

			for(uint32_t i = 1; i < this->mip_levels; i++){
				barrier.subresourceRange.baseMipLevel = i - 1;
				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				vkCmdPipelineBarrier(
					command_buffer.handle,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier
				);

				const auto blit = VkImageBlit{
					.srcSubresource = {
						.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
						.mipLevel       = i - 1,
						.baseArrayLayer = 0,
						.layerCount     = 1,
					},
					.srcOffsets     = {{0, 0, 0}, {mip_width, mip_height, 1}},

					.dstSubresource = {
						.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
						.mipLevel       = i,
						.baseArrayLayer = 0,
						.layerCount     = 1,
					},
					.dstOffsets     = {{0, 0, 0}, { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 }},
				};

				vkCmdBlitImage(
					command_buffer.handle,
					this->handle, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					this->handle, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					1, &blit,
					VK_FILTER_LINEAR
				);

				barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
				barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				vkCmdPipelineBarrier(
					command_buffer.handle,
					VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
					0, nullptr,
					0, nullptr,
					1, &barrier
				);

				if(mip_width > 1) mip_width /= 2;
				if(mip_height > 1) mip_height /= 2;
			}

			barrier.subresourceRange.baseMipLevel = this->mip_levels - 1;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(command_buffer.handle,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);

			if(command_buffer.end_and_free_single_use(device, command_pool, queue) == false) return false;

			PH_TRACE("Generated Vulkan texture mip maps");
			return true;
		};





		//////////////////////////////////////////////////////////////////////
		// texture


		auto Texture::create(
			const class Device& device,
			const VkCommandPool& command_pool,
			const evo::ArrayProxy<byte> data,
			uint32_t width,
			uint32_t height,
			bool create_mipmaps
		) noexcept -> bool {

			///////////////////////////////////
			// image

			constexpr VkFormat image_format = VK_FORMAT_R8G8B8A8_UNORM;


			PH_ASSERT(data.size() == (width * height * 4), "image data does not match image size");

			auto staging_buffer = vulkan::Buffer{};
			if(staging_buffer.create(
				device,
				data.size(),
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				true
			) == false){
				PH_ERROR("Failed to create image staging buffer");
				return false;
			};


				staging_buffer.set_data(data.data());


				uint32_t mip_levels = 1;
				if(create_mipmaps){
					mip_levels = uint32_t(std::floor(std::log2(std::max(width, height)))) + 1;
				}


				if(this->image.create(
					device,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					width,
					height,
					image_format,
					VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
					mip_levels,
					VK_SAMPLE_COUNT_1_BIT,
					VK_IMAGE_TILING_OPTIMAL,
					VK_IMAGE_ASPECT_COLOR_BIT
				) == false){
					PH_ERROR("Failed to create image on GPU");
					return false;
				}


				if(this->image.transition_layout(
					device,
					command_pool,
					device.get_graphics_queue(),
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
				) == false){
					PH_ERROR("Failed to transition image layout for transfer");
					return false;
				}

				if(this->image.copy_from_buffer(device, command_pool, device.get_graphics_queue(), staging_buffer) == false){
					PH_ERROR("Failed load image to gpu");
					return false;
				};

			staging_buffer.destroy(device);

			if(create_mipmaps){
				// generating mipmaps transitions the image to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				if(this->image.generate_mipmaps(device, command_pool, device.get_graphics_queue(), image_format) == false){
					PH_ERROR("Failed to generate image mipmaps");
					return false;
				}

			}else{
				if(this->image.transition_layout(
					device,
					command_pool,
					device.get_graphics_queue(),
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
				) == false){
					PH_ERROR("Failed to transition image layout for reading");
					return false;
				};
			}



			///////////////////////////////////
			// sampler

			auto sampler_result = vulkan::create_image_sampler(device, mip_levels);

			if(sampler_result.has_value() == false){
				PH_ERROR("Failed to create sampler for texture");
				return false;
			}

			this->sampler = std::move(*sampler_result);



			///////////////////////////////////
			// done

			PH_DEBUG("Created: Vulkan Texture");
			return true;
		};




		auto Texture::destroy(const Device& device) noexcept -> void {
			vulkan::destroy_image_sampler(device, this->sampler);
			this->image.destroy(device);

			PH_DEBUG("Destroyed: Vulkan Texture");
		};

		
	};
};