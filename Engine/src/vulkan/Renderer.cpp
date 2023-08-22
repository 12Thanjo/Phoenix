#include "Renderer.h"


#include "Logging.h"
#include <format>


#include "vulkan/utils.h"

namespace ph{
	namespace vulkan{
		

		auto Renderer::init(GLFW::Window* window) noexcept -> bool {

			///////////////////////////////////
			// instance

			{
				auto instance_result = vulkan::create_instance("Playground");
				if(instance_result.has_value() == false){
					PH_FATAL("Failed to create Vulkan instance");
					return false;
				}

				this->instance = std::move(*instance_result);
			}


			///////////////////////////////////
			// debug messenger

			#if defined(PH_CONFIG_DEBUG)
				{
					auto debug_messenger_result = vulkan::create_debug_messenger(this->instance);
					if(debug_messenger_result.has_value() == false){
						PH_FATAL("Failed to create Vulkan debug messenger");
						return false;
					}

					this->debug_messenger = std::move(*debug_messenger_result);
				}
			#endif


			///////////////////////////////////
			// surface

			{
				auto surface_result = window->create_surface(this->instance);
				if(surface_result.has_value() == false){
					PH_FATAL("Failed to create Vulkan surface");
					return false;
				}

				this->surface = std::move(*surface_result);
			}


			///////////////////////////////////
			//  device

			auto physical_device_result = vulkan::select_physical_device(this->instance);
			if(physical_device_result.has_value() == false){
				PH_FATAL("Failed to pick Vulkan physical device");
				return false;
			}

			this->msaa_samples = vulkan::get_max_msaa_samples(*physical_device_result);


			if(this->device.create(this->surface, *physical_device_result, this->use_sample_shading) == false){
				PH_FATAL("Failed to create Vulkan logical device");
				return false;
			}


			///////////////////////////////////
			// swapchain

			{
				const auto [window_width, window_height] = window->size();
				if(this->swapchain.create(this->device, this->surface, window_width, window_height) == false){
					PH_FATAL("Failed to create swapchain");
					return false;
				}
			}

			
			///////////////////////////////////
			// 3D

			// TODO: rewrite this system (including error checking)

			{
				const auto result = this->render_pass_3D.add_descriptor_set_layout(this->device, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
				});

				if(result.has_value() == false){ return false; }

				this->global_descriptor_set_layout_3D = *result;
			}


			{
				const auto result = this->render_pass_3D.add_descriptor_set_layout(this->device, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
				});

				if(result.has_value() == false){ return false; }

				this->instance_descriptor_set_layout_3D = *result;
			}

			


			{
				const bool render_pass_3D_result = this->render_pass_3D.create(
					this->device,
					this->msaa_samples,
					false,
					true,
					std::initializer_list<Attachment>{
						{ vulkan::AttachmentType::Color,        this->swapchain.get_image_format(),                     {0.0f, 0.1f, 0.2f, 1.0f} },
						{ vulkan::AttachmentType::DepthStencil, vulkan::find_depth_format(this->device.get_physical()), 1.0f, 0                  },
						{ vulkan::AttachmentType::Resolve,      this->swapchain.get_image_format() 					    /* none */			     },
					},
					std::initializer_list<VkPushConstantRange>{
						{VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4)},
					}
				);

				if(render_pass_3D_result == false){
					PH_FATAL("Failed to create 3D render pass");
					return false;
				}
			}

			

			{

				const auto vert_shader_bytecode = vulkan::load_shader_code("assets/shaders/basic3D.vert.spv");
				if(vert_shader_bytecode.has_value() == false){
					PH_FATAL("Failed to load 3D vertex shader");
					return false;
				}


				const auto frag_shader_bytecode = vulkan::load_shader_code("assets/shaders/basic3D.frag.spv");
				if(frag_shader_bytecode.has_value() == false){
					PH_FATAL("Failed to load 3D fragment shader");
					return false;
				}

				VkShaderModule vert_shader_module = vulkan::create_shader_module(this->device, *vert_shader_bytecode).value();
				VkShaderModule frag_shader_module = vulkan::create_shader_module(this->device, *frag_shader_bytecode).value();


				auto pipeline_config = PipelineConfig{
					.msaa_samples       = this->msaa_samples,
					.use_sample_shading = this->use_sample_shading,
				};
				pipeline_config.add_vertex_shader(vert_shader_module);
				pipeline_config.add_fragment_shader(frag_shader_module);
				pipeline_config.add_vertex_binding(sizeof(vulkan::Vertex3D), VK_VERTEX_INPUT_RATE_VERTEX);
				pipeline_config.add_vertex_attribute(vulkan::format<float, 3>());
				pipeline_config.add_vertex_attribute(vulkan::format<float, 2>());
				pipeline_config.add_vertex_attribute(vulkan::format<float, 3>());


				if(this->render_pass_3D.create_pipeline(this->device, pipeline_config, VK_NULL_HANDLE).has_value() == false){
					PH_ERROR("Failed to create 3D render pipeline");
					return false;
				}

				vulkan::destroy_shader_module(this->device, frag_shader_module);
				vulkan::destroy_shader_module(this->device, vert_shader_module);
			}


			///////////////////////////////////
			// 2D

			{
				const bool render_pass_2D_result = this->render_pass_2D.create(this->device, this->msaa_samples, true, false, {
					{ vulkan::AttachmentType::Color,   this->swapchain.get_image_format() },
					{ vulkan::AttachmentType::Resolve, this->swapchain.get_image_format() },
				});

				if(render_pass_2D_result == false){
					PH_FATAL("Failed to create 2D render pass");
					return false;
				}


				this->global_descriptor_set_layout_2D = *vulkan::create_descriptor_set_layout(this->device, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT},
				});

				this->instance_descriptor_set_layout_2D = *vulkan::create_descriptor_set_layout(this->device, {
					{0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
					{1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT},
				});


				this->pipeline_layout_2D = vulkan::create_pipeline_layout(
					this->device,
					{
						this->global_descriptor_set_layout_2D,
						this->instance_descriptor_set_layout_2D,
					},
					{
						{VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4)},
					}
				).value();


				const auto vert_shader_bytecode = vulkan::load_shader_code("assets/shaders/basic2D.vert.spv");
				if(vert_shader_bytecode.has_value() == false){
					PH_FATAL("Failed to load 2D vertex shader");
					return false;
				}


				const auto frag_shader_bytecode = vulkan::load_shader_code("assets/shaders/basic2D.frag.spv");
				if(frag_shader_bytecode.has_value() == false){
					PH_FATAL("Failed to load 2D fragment shader");
					return false;
				}

				VkShaderModule vert_shader_module = vulkan::create_shader_module(this->device, *vert_shader_bytecode).value();
				VkShaderModule frag_shader_module = vulkan::create_shader_module(this->device, *frag_shader_bytecode).value();



				auto pipeline_config = PipelineConfig{
					.msaa_samples       = this->msaa_samples,
					.use_sample_shading = this->use_sample_shading,
				};
				pipeline_config.add_vertex_shader(vert_shader_module);
				pipeline_config.add_fragment_shader(frag_shader_module);
				pipeline_config.add_vertex_binding(sizeof(vulkan::Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
				pipeline_config.add_vertex_attribute(vulkan::format<float, 2>());
				pipeline_config.add_vertex_attribute(vulkan::format<float, 2>());


				if(this->pipeline_2D.create(
					this->device,
					this->render_pass_2D,
					this->render_pass_3D.pipeline_layout,
					pipeline_config,
					VK_NULL_HANDLE
				) == false){
					PH_FATAL("Failed to create 2D render pipeline");
					return false;
				}

				vulkan::destroy_shader_module(this->device, frag_shader_module);
				vulkan::destroy_shader_module(this->device, vert_shader_module);
			}






			///////////////////////////////////
			// render targets

			if(this->create_render_targets() == false){
				PH_FATAL("Failed to create render targets");
				return false;
			}


			///////////////////////////////////
			// command pool

			{
				const vulkan::QueueFamilyIndices queue_family_indices = vulkan::find_queue_families(this->device.get_physical(), this->surface);
				if(queue_family_indices.is_complete() == false){
					PH_FATAL("Failed to find all required queue families");
					return false;
				}


				auto command_pool_result = vulkan::create_command_pool(this->device, *queue_family_indices.graphics_family);
				if(command_pool_result.has_value() == false){
					PH_FATAL("Failed to create command pool");
					return false;
				}

				this->command_pool = std::move(*command_pool_result);
			}
			
			

			///////////////////////////////////
			// uniform buffers

			for(auto& uniform_buffer : this->global_uniform_buffers_3D){
				bool result = uniform_buffer.create(
					this->device, 
					sizeof(GlobalUBO3D),
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					true
				);

				if(result == false){
					PH_FATAL("Failed to create uniform buffer");
					return false;
				}
			}


			for(auto& uniform_buffer_list : this->instance_uniform_buffers_3D){
				for(auto& uniform_buffer : uniform_buffer_list){
					bool result = uniform_buffer.create(
						this->device, 
						sizeof(InstanceUBO3D),
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						true
					);

					if(result == false){
						PH_FATAL("Failed to create uniform buffer");
						return false;
					}
				}
			}





			for(auto& uniform_buffer : this->global_uniform_buffers_2D){
				bool result = uniform_buffer.create(
					this->device, 
					sizeof(GlobalUBO2D),
					VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
					true
				);

				if(result == false){
					PH_FATAL("Failed to create uniform buffer");
					return false;
				}
			}


			for(auto& uniform_buffer_list : this->instance_uniform_buffers_2D){
				for(auto& uniform_buffer : uniform_buffer_list){
					bool result = uniform_buffer.create(
						this->device, 
						sizeof(InstanceUBO2D),
						VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
						VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
						true
					);

					if(result == false){
						PH_FATAL("Failed to create uniform buffer");
						return false;
					}
				}
			}


			///////////////////////////////////
			// descriptor pool


			{
				const auto descriptor_pool_result = vulkan::create_descriptor_pool(
					this->device,
					((MAX_FRAMES_IN_FLIGHT * MAX_DESCRIPTOR_SETS) + MAX_FRAMES_IN_FLIGHT),
					{
						VkDescriptorPoolSize{
							VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
							((MAX_FRAMES_IN_FLIGHT * MAX_DESCRIPTOR_SETS) + MAX_FRAMES_IN_FLIGHT),
						},

						VkDescriptorPoolSize{
							VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
							(MAX_FRAMES_IN_FLIGHT * MAX_DESCRIPTOR_SETS),
						},
					}
				);

				if(descriptor_pool_result.has_value() == false){
					PH_FATAL("Failed to create descriptor pool");
					return false;
				}

				this->descriptor_pool = std::move(*descriptor_pool_result);
			}

			

			///////////////////////////////////
			// command buffers

			for(auto& command_buffer : this->command_buffers){
				if(command_buffer.allocate(this->device, this->command_pool) == false){
					PH_FATAL("Failed to allocate a command buffer");
					return false;
				}
			}


			///////////////////////////////////
			// synchronization


			for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i+=1){
				if(
					this->image_available_semaphores[i].create(this->device) == false
					|| this->render_finished_semaphores[i].create(this->device) == false
					|| this->in_flight_fences[i].create(this->device) == false
				){
					PH_FATAL("Failed to create Vulkan syncronization objects");
				}
			}



			///////////////////////////////////
			// global descriptor sets 3D

			for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i+=1){
				const auto result = this->render_pass_3D.allocate_descriptor_set(this->device, this->global_descriptor_set_layout_3D);
				if(result.has_value() == false) return false;

				this->global_descriptor_sets_3D[i] = *result;
			}


			for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
				this->render_pass_3D.write_descriptor_set_ubo(
					this->device, this->global_descriptor_set_layout_3D, this->global_descriptor_sets_3D[i], 0, this->global_uniform_buffers_3D[i]
				);
			}



			///////////////////////////////////
			// instance descriptor sets 3D


			for(int i = 0; i < MAX_DESCRIPTOR_SETS; i+=1){
				for(size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++){
					const auto result = this->render_pass_3D.allocate_descriptor_set(this->device, this->instance_descriptor_set_layout_3D);
					if(result.has_value() == false) return false;

					this->instance_descriptor_sets_3D[i][j] = *result;
				}
			}


			for(int i = 0; i < MAX_DESCRIPTOR_SETS; i+=1){
				for(size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++){
					this->render_pass_3D.write_descriptor_set_ubo(
						this->device, this->instance_descriptor_set_layout_3D, this->instance_descriptor_sets_3D[i][j], 0, this->instance_uniform_buffers_3D[i][j]
					);
				}
			}


			///////////////////////////////////
			// global descriptor sets 2D

			{
				auto layouts = evo::StaticVector<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT>();


				for(int i = 0; i < layouts.capacity(); i+=1){
					layouts.push_back(this->global_descriptor_set_layout_2D);
				}


				auto allocated_descriptor_sets = vulkan::allocate_descriptor_sets(this->device, this->descriptor_pool, layouts);

				if(allocated_descriptor_sets.has_value() == false){
					PH_FATAL("Failed to allocate descriptor sets");
					return false;
				}

				for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i+=1){
					this->global_descriptor_sets_2D[i] = (*allocated_descriptor_sets)[i];
				}


				for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
					const auto buffer_info = VkDescriptorBufferInfo{
						.buffer = this->global_uniform_buffers_2D[i].handle,
						.offset = 0,
						.range  = sizeof(GlobalUBO2D),
					};

					
					const auto descriptor_writes = std::array<VkWriteDescriptorSet, 1>{
						VkWriteDescriptorSet{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
							.dstSet          = this->global_descriptor_sets_2D[i],
							.dstBinding      = 0,
							.dstArrayElement = 0,

							.descriptorCount = 1,
							.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
							.pBufferInfo     = &buffer_info,
						},
					};


					vkUpdateDescriptorSets(
						this->device.get_handle(), uint32_t(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr
					);
				}
			}



			///////////////////////////////////
			// instance descriptor sets 2D

			{
				auto layouts = evo::StaticVector<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT * MAX_DESCRIPTOR_SETS>();



				for(int i = 0; i < layouts.capacity(); i+=1){
					layouts.push_back(this->instance_descriptor_set_layout_2D);
				}


				auto allocated_descriptor_sets = vulkan::allocate_descriptor_sets(this->device, this->descriptor_pool, layouts);

				if(allocated_descriptor_sets.has_value() == false){
					PH_FATAL("Failed to allocate descriptor sets");
					return false;
				}

				for(int i = 0; i < MAX_DESCRIPTOR_SETS; i+=1){
					for(int j = 0; j < MAX_FRAMES_IN_FLIGHT; j+=1){
						this->instance_descriptor_sets_2D[i][j] = (*allocated_descriptor_sets)[(i * MAX_FRAMES_IN_FLIGHT) + j];
					}
				}


				for(size_t i = 0; i < 5; i+=1){
					for(size_t j = 0; j < MAX_FRAMES_IN_FLIGHT; j++){
						const auto buffer_info = VkDescriptorBufferInfo{
							.buffer = this->instance_uniform_buffers_2D[i][j].handle,
							.offset = 0,
							.range  = sizeof(InstanceUBO2D),
						};


						const auto descriptor_writes = std::array<VkWriteDescriptorSet, 1>{
							VkWriteDescriptorSet{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
								.dstSet          = this->instance_descriptor_sets_2D[i][j],
								.dstBinding      = 0,
								.dstArrayElement = 0,

								.descriptorCount = 1,
								.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
								.pBufferInfo     = &buffer_info,
							},
						};


						vkUpdateDescriptorSets(
							this->device.get_handle(), uint32_t(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr
						);
					}
				}
			}












			///////////////////////////////////
			// vertex / index buffers


			{ // 3D
				bool buffer_result = this->vertex_buffer_3D.create(
					this->device,
					1024 * 1024 * 10, // sizeof(vulkan::Vertex3D) * 1024 * 1024 * 30,
					VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					false
				);

				if(buffer_result == false){
					PH_ERROR("Failed to create vertex buffer");
					return false;
				}


				buffer_result = this->index_buffer_3D.create(
					this->device,
					1024 * 1024 * 10, // sizeof(uint32_t) * 1024 * 1024 * 30,
					VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					false
				);

				if(buffer_result == false){
					PH_ERROR("Failed to create index buffer");
					return false;
				}
			}


			{ // 2D
				bool buffer_result = this->vertex_buffer_2D.create(
					this->device,
					100,
					VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					false
				);

				if(buffer_result == false){
					PH_ERROR("Failed to create vertex buffer");
					return false;
				}


				buffer_result = this->index_buffer_2D.create(
					this->device,
					100,
					VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
					false
				);

				if(buffer_result == false){
					PH_ERROR("Failed to create index buffer");
					return false;
				}
			}



			///////////////////////////////////
			// done

			PH_INFO("Initialized: Renderer");

			return true;
		};







		auto Renderer::shutdown() noexcept -> void {
			if(this->device.wait_idle() == false){
				PH_FATAL("Failed to wait for Vulkan device to become idle");
			}


			for(auto& texture : this->textures){
				texture.destroy(this->device);
			}


			this->destroy_render_targets();
			this->swapchain.destroy(this->device);



			for(auto& uniform_buffer : this->global_uniform_buffers_3D){
				uniform_buffer.destroy(this->device);
			}

			for(auto& uniform_buffer_list : this->instance_uniform_buffers_3D){
				for(auto& uniform_buffer : uniform_buffer_list){
					uniform_buffer.destroy(this->device);
				}
			}

			for(auto& uniform_buffer : this->global_uniform_buffers_2D){
				uniform_buffer.destroy(this->device);
			}

			for(auto& uniform_buffer_list : this->instance_uniform_buffers_2D){
				for(auto& uniform_buffer : uniform_buffer_list){
					uniform_buffer.destroy(this->device);
				}
			}


			vulkan::destroy_descriptor_pool(this->device, this->descriptor_pool);


			vulkan::destroy_descriptor_set_layout(this->device, this->instance_descriptor_set_layout_2D);
			vulkan::destroy_descriptor_set_layout(this->device, this->global_descriptor_set_layout_2D);


			this->index_buffer_2D.destroy(this->device);
			this->vertex_buffer_2D.destroy(this->device);

			this->index_buffer_3D.destroy(this->device);
			this->vertex_buffer_3D.destroy(this->device);


			this->render_pass_3D.destroy(this->device);

			this->pipeline_2D.destroy(this->device);
			vulkan::destroy_pipeline_layout(this->device, this->pipeline_layout_2D);
			this->render_pass_2D.destroy(this->device);



			for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
				this->in_flight_fences[i].destroy(this->device);
				this->render_finished_semaphores[i].destroy(this->device);
				this->image_available_semaphores[i].destroy(this->device);
			}

			vulkan::destroy_command_pool(this->device, this->command_pool);

			this->device.destroy();

			#if defined(PH_CONFIG_DEBUG)
				vulkan::destroy_debug_messenger(this->instance, this->debug_messenger);
			#endif

			vkDestroySurfaceKHR(instance, surface, nullptr);
			
			vulkan::destroy_instance(this->instance);

			

			PH_INFO("Shutdown: Renderer");
		}





		auto Renderer::begin_frame() noexcept -> FrameResult {
			const auto& in_flight_fence = this->in_flight_fences[this->current_frame];

			if(in_flight_fence.wait(this->device) == false){
				PH_ERROR("Faild to wait for in flight fence");
				return FrameResult::Error;
			}


			const auto result = this->swapchain.acquire_next_image(this->device, this->image_available_semaphores[this->current_frame]);

			if(result == vulkan::Swapchain::Result::NeedResize){
				PH_INFO("Swapchain needs to be resized");
				return FrameResult::NeedResize;

			}else if(result == vulkan::Swapchain::Result::Error){
				return FrameResult::Error;
			}



			if(in_flight_fence.reset(this->device) == false){
				PH_ERROR("Faild to reset in flight fence");
				return FrameResult::Error;
			}

			const auto& command_buffer = this->command_buffers[this->current_frame];

			command_buffer.reset();

			if(command_buffer.begin(false, false, false) == false){
				PH_ERROR("Failed to begin command buffer");
				return FrameResult::Error;
			}

			const auto viewport = VkViewport{
				.x        = 0.0f,
				.y        = 0.0f,
				.width    = float(this->swapchain.get_width()),
				.height   = float(this->swapchain.get_height()),
				.minDepth = 0.0f,
				.maxDepth = 1.0f,
			};
			command_buffer.set_viewport(viewport);

			const auto scissor = VkRect2D{
				.offset = {0, 0},
				.extent = {this->swapchain.get_width(), this->swapchain.get_height()},
			};
			command_buffer.set_scissor(scissor);


			return FrameResult::Success;
		};




		auto Renderer::end_frame() noexcept -> FrameResult {

			const auto& command_buffer = this->command_buffers[this->current_frame];

			if(command_buffer.end() == false){
				PH_ERROR("Failed to end command buffer");
				return FrameResult::Error;
			}


			///////////////////////////////////
			// submit

			const auto wait_semaphores = std::array<VkSemaphore, 1>{
				this->image_available_semaphores[this->current_frame].handle
			};

			const auto wait_stages = std::array<VkPipelineStageFlags, 1>{
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			};

			const auto signal_semaphores = std::array<VkSemaphore, 1>{this->render_finished_semaphores[this->current_frame].handle};

			const auto submit_info = VkSubmitInfo{ .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
				.waitSemaphoreCount   = 1,
				.pWaitSemaphores      = wait_semaphores.data(),
				.pWaitDstStageMask    = wait_stages.data(),

				.commandBufferCount   = 1,
				.pCommandBuffers      = &command_buffer.handle,

				.signalSemaphoreCount = 1,
				.pSignalSemaphores    = signal_semaphores.data(),
			};



			const VkResult submit_result = vkQueueSubmit(
				this->device.get_graphics_queue(), 1, &submit_info, this->in_flight_fences[this->current_frame].handle
			);
			if(vulkan::result_fail(submit_result, "vkQueueSubmit")){
				PH_FATAL("Failed to submit draw command buffer");
				return FrameResult::Error;
			}



			///////////////////////////////////
			// present


			const vulkan::Swapchain::Result present_result = this->swapchain.present(
				this->device.get_graphics_queue(), this->render_finished_semaphores[this->current_frame]
			);

			this->current_frame = ((this->current_frame + 1) % MAX_FRAMES_IN_FLIGHT);

			if(present_result == vulkan::Swapchain::Result::NeedResize){
				return FrameResult::NeedResize;

			}else if(present_result == vulkan::Swapchain::Result::Error){
				return FrameResult::Error;
			}
			


			///////////////////////////////////
			// done


			return FrameResult::Success;
		};





		auto Renderer::create_texture(
			const evo::ArrayProxy<byte> data, uint32_t width, uint32_t height, bool create_mipmaps
		) noexcept -> std::optional<uint64_t> {

			const uint64_t texture_index = this->textures.size();
			auto& new_texture = this->textures.emplace_back();

			if(new_texture.create(this->device, this->command_pool, data, width, height, create_mipmaps) == false){
				PH_ERROR("Failed to create new Texture");
				new_texture.destroy(this->device);
				this->textures.pop_back();
				return std::nullopt;
			}

			return texture_index;
		};




		template<typename T>
		auto Renderer::create_mesh_impl(
			const evo::ArrayProxy<T> vbo, const evo::ArrayProxy<uint32_t> ibo,
			vulkan::Buffer& vertex_buffer, uint32_t& vertex_buffer_index,
			vulkan::Buffer& index_buffer, uint32_t& index_buffer_index
		) noexcept -> std::optional< std::pair<uint32_t, uint32_t> > { // vertex offset / index offset

			const auto output = std::pair<uint32_t, uint32_t>{vertex_buffer_index, index_buffer_index};


			const uint32_t vbo_size = uint32_t(sizeof(T) * vbo.size());
			const uint32_t ibo_size = uint32_t(sizeof(uint32_t) * ibo.size());
			const uint32_t staging_buffer_size = std::max(vbo_size, ibo_size);


			auto staging_buffer = vulkan::Buffer{};

			bool result = staging_buffer.create(
				this->device,
				staging_buffer_size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				true
			);
			if(result == false){
				PH_ERROR("Failed to create staging buffer for creating mesh");
				return std::nullopt;
			}


			staging_buffer.set_data(vbo.data(), 0, vbo_size);
			result = vertex_buffer.copy_from(
				this->device, this->command_pool, this->device.get_transfer_queue(), staging_buffer, vbo_size, 0, sizeof(T) * vertex_buffer_index
			);
			if(result == false){
				PH_ERROR("Failed to upload mesh vertex buffer");
				return std::nullopt;
			}


			staging_buffer.set_data(ibo.data(), 0, ibo_size);
			result = index_buffer.copy_from(
				this->device, this->command_pool, this->device.get_transfer_queue(), staging_buffer, ibo_size, 0, sizeof(uint32_t) * index_buffer_index
			);
			if(result == false){
				PH_ERROR("Failed to upload mesh index buffer");
				return std::nullopt;
			}

			staging_buffer.destroy(this->device);

			vertex_buffer_index += uint32_t(vbo.size());
			index_buffer_index += uint32_t(ibo.size());

			// PH_TRACE("Created mesh 3D");
			return output;
		};




		auto Renderer::create_mesh_3D(
			const evo::ArrayProxy<vulkan::Vertex3D> vbo, const evo::ArrayProxy<uint32_t> ibo
		) noexcept -> std::optional< std::pair<uint32_t, uint32_t> > {

			auto output = this->create_mesh_impl(
				vbo, ibo,
				this->vertex_buffer_3D, this->vertex_buffer_3D_index,
				this->index_buffer_3D, this->index_buffer_3D_index
			);

			if(output.has_value() == false){
				return std::nullopt;
			}


			PH_TRACE("Created mesh 3D");
			return *output;
		};



		auto Renderer::create_mesh_2D(
			const evo::ArrayProxy<vulkan::Vertex2D> vbo, const evo::ArrayProxy<uint32_t> ibo
		) noexcept -> std::optional< std::pair<uint32_t, uint32_t> > {

			auto output = this->create_mesh_impl<vulkan::Vertex2D>(
				vbo, ibo,
				this->vertex_buffer_2D, this->vertex_buffer_2D_index,
				this->index_buffer_2D, this->index_buffer_2D_index
			);

			if(output.has_value() == false){
				return std::nullopt;
			}


			PH_TRACE("Created mesh 2D");
			return *output;
		};











		auto Renderer::create_render_targets() noexcept -> bool {
			bool result = this->color_image.create(
				this->device,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				this->swapchain.get_width(),
				this->swapchain.get_height(),
				this->swapchain.get_image_format(),
				VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				1,
				this->msaa_samples,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_ASPECT_COLOR_BIT
			);

			if(result == false){
				PH_ERROR("Failed to create color render target");
				return false;
			}


			result = this->depth_image.create(
				this->device,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				this->swapchain.get_width(),
				this->swapchain.get_height(),
				vulkan::find_depth_format(this->device.get_physical()),
				VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
				1,
				this->msaa_samples,
				VK_IMAGE_TILING_OPTIMAL,
				VK_IMAGE_ASPECT_DEPTH_BIT
			);

			if(result == false){
				PH_ERROR("Failed to create depth render target");
				return false;
			}


			///////////////////////////////////
			// framebuffers

			this->framebuffers_3D.resize(this->swapchain.get_image_count());

			for(uint32_t i = 0; i < uint32_t(this->swapchain.get_image_count()); i++){
				const auto attachments = std::array<VkImageView, 3>{
					this->color_image.view,
					this->depth_image.view,
					this->swapchain.get_image_view(i)
				};

				result = this->framebuffers_3D[i].create(
					this->device, this->render_pass_3D.render_pass, this->swapchain.get_width(), this->swapchain.get_height(), attachments
				);

				if(result == false){
					PH_ERROR("Failed to create framebuffer for 3D rendering");
					return false;
				}
			}

			this->framebuffers_2D.resize(this->swapchain.get_image_count());
			for(uint32_t i = 0; i < uint32_t(this->swapchain.get_image_count()); i++){
				const auto attachments = std::array<VkImageView, 2>{
					this->color_image.view,
					this->swapchain.get_image_view(i)
				};

				result = this->framebuffers_2D[i].create(
					this->device, this->render_pass_2D, this->swapchain.get_width(), this->swapchain.get_height(), attachments
				);

				if(result == false){
					PH_ERROR("Failed to create framebuffer for 2D rendering");
					return false;
				}
			}

			return true;
		};


		auto Renderer::destroy_render_targets() noexcept -> void {
			this->depth_image.destroy(this->device);
			this->color_image.destroy(this->device);

			for(auto& framebuffer : this->framebuffers_2D){
				framebuffer.destroy(this->device);
			}

			for(auto& framebuffer : this->framebuffers_3D){
				framebuffer.destroy(this->device);
			}

		};


		auto Renderer::resize(uint32_t width, uint32_t height) noexcept -> bool {

			PH_TRACE(std::format("Resized window to: ({}, {})", width, height));


			if(this->device.wait_idle() == false){
				PH_FATAL("Failed to wait for device to become idle");
				return false;
			}

			this->destroy_render_targets();

			if(this->swapchain.resize(this->device, this->surface, width, height) == false){
				PH_ERROR("Failed to resize swapchain");
				return false;
			}

			if(this->create_render_targets() == false){
				PH_ERROR("Failed to create render targets");
				return false;
			}


			PH_TRACE("Resized renderer");
			return true;
		};





		auto Renderer::set_global_ubo_3D(const void* data) noexcept -> void {
			this->global_uniform_buffers_3D[this->current_frame].set_data(data);
		};


		auto Renderer::set_instance_ubo_3D(uint32_t index, const void* data) noexcept -> void {
			this->instance_uniform_buffers_3D[index][this->current_frame].set_data(data);
		};


		auto Renderer::set_global_ubo_2D(const void* data) noexcept -> void {
			this->global_uniform_buffers_2D[this->current_frame].set_data(data);
		};


		auto Renderer::set_instance_ubo_2D(uint32_t index, const void* data) noexcept -> void {
			this->instance_uniform_buffers_2D[index][this->current_frame].set_data(data);
		};




		auto Renderer::set_instance_texture_3D(uint32_t descriptor_index, uint32_t texture_index) noexcept -> void {
			this->render_pass_3D.write_descriptor_set_texture(
				this->device,
				this->instance_descriptor_set_layout_3D,
				this->instance_descriptor_sets_3D[descriptor_index][this->current_frame],
				1,
				this->textures[texture_index]
			);
		};


		auto Renderer::set_instance_texture_2D(uint32_t descriptor_index, uint32_t texture_index) noexcept -> void {
			const auto image_info = VkDescriptorImageInfo{
				.sampler     = this->textures[texture_index].get_sampler(),
				.imageView   = this->textures[texture_index].get_image_view(),
				.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			};

			const auto descriptor_writes = std::array<VkWriteDescriptorSet, 1>{
				VkWriteDescriptorSet{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstSet          = this->instance_descriptor_sets_2D[descriptor_index][this->current_frame],
					.dstBinding      = 1,
					.dstArrayElement = 0,

					.descriptorCount = 1,
					.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.pImageInfo      = &image_info,
				},
			};

			vkUpdateDescriptorSets(
				this->device.get_handle(), uint32_t(descriptor_writes.size()), descriptor_writes.data(), 0, nullptr
			);
		};




		auto Renderer::begin_render_pass_3D() noexcept -> void {
			const vulkan::CommandBuffer& command_buffer = this->command_buffers[this->current_frame];

			this->render_pass_3D.render_pass.begin(
				command_buffer,
				this->framebuffers_3D[this->swapchain.get_image_index()],
				this->swapchain.get_width(),
				this->swapchain.get_height()
			);

			command_buffer.bind_pipeline(this->render_pass_3D.pipelines[0].handle, VK_PIPELINE_BIND_POINT_GRAPHICS);


			this->render_pass_3D.bind_descriptor_set(
				command_buffer, this->global_descriptor_set_layout_3D, this->global_descriptor_sets_3D[this->current_frame]
			);
		};


		auto Renderer::end_render_pass_3D() noexcept -> void {
			this->render_pass_3D.render_pass.end(this->command_buffers[this->current_frame]);
		};




		auto Renderer::begin_render_pass_2D() noexcept -> void {
			const vulkan::CommandBuffer& command_buffer = this->command_buffers[this->current_frame];

			this->render_pass_2D.begin(
				command_buffer, this->framebuffers_2D[this->swapchain.get_image_index()], this->swapchain.get_width(), this->swapchain.get_height()
			);

			command_buffer.bind_pipeline(this->pipeline_2D.handle, VK_PIPELINE_BIND_POINT_GRAPHICS);

			command_buffer.bind_descriptor_set(
				this->pipeline_layout_2D, 0, this->global_descriptor_sets_2D[this->current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS
			);
		};


		auto Renderer::end_render_pass_2D() noexcept -> void {
			this->render_pass_2D.end(this->command_buffers[this->current_frame]);
		};




		auto Renderer::bind_descriptor_set_3D(uint32_t index) noexcept -> void {
			this->render_pass_3D.bind_descriptor_set(this->command_buffers[this->current_frame], this->instance_descriptor_set_layout_3D, this->instance_descriptor_sets_3D[index][this->current_frame]);
		};

		auto Renderer::bind_descriptor_set_2D(uint32_t index) noexcept -> void {
			this->command_buffers[this->current_frame].bind_descriptor_set(
				this->pipeline_layout_2D, 1, this->instance_descriptor_sets_2D[index][this->current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS
			);
		};



		auto Renderer::set_model_push_constant_3D(const glm::mat4& model) noexcept -> void {
			this->command_buffers[this->current_frame].push_constant(
				this->render_pass_3D.pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), &model
			);
		};

		auto Renderer::set_model_push_constant_2D(const glm::mat4& model) noexcept -> void {
			this->command_buffers[this->current_frame].push_constant(
				this->pipeline_layout_2D, VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), &model
			);
		};



		auto Renderer::bind_vertex_buffer_3D() noexcept -> void {
			this->command_buffers[this->current_frame].bind_vertex_buffer(this->vertex_buffer_3D, 0);
		};

		auto Renderer::bind_index_buffer_3D() noexcept -> void {
			this->command_buffers[this->current_frame].bind_index_buffer(this->index_buffer_3D, 0);
		};


		auto Renderer::bind_vertex_buffer_2D() noexcept -> void {
			this->command_buffers[this->current_frame].bind_vertex_buffer(this->vertex_buffer_2D, 0);
		};

		auto Renderer::bind_index_buffer_2D() noexcept -> void {
			this->command_buffers[this->current_frame].bind_index_buffer(this->index_buffer_2D, 0);
		};


		auto Renderer::draw_indexed(uint32_t index_count, uint32_t first_index, int32_t vertex_offset) noexcept -> void {
			this->command_buffers[this->current_frame].draw_indexed(index_count, first_index, vertex_offset);
		};



	};
};