#pragma once


#include <libs/Evo/Evo.h>
#include <libs/glm/glm.h>
#include <libs/Vulkan/Vulkan.h>

#include "Logging.h"
#include "GLFW.h"


#include "vulkan/Context.h"
#include "vulkan/utils.h"
#include "vulkan/Framebuffer.h"
#include "vulkan/images.h"
#include "vulkan/Buffer.h"
#include "vulkan/commands.h"
#include "vulkan/Swapchain.h"
#include "vulkan/RenderPass.h"
#include "vulkan/Pipeline.h"
#include "vulkan/synchronization.h"




namespace ph{
	namespace vulkan{



		class Renderer{
			public:
				EVO_NODISCARD auto init(GLFW::Window* window) noexcept -> bool;
				auto shutdown() noexcept -> void;
				

				enum class FrameResult{
					Success,
					NeedResize,
					Error,
				};
				
				EVO_NODISCARD auto begin_frame() noexcept -> FrameResult;
				EVO_NODISCARD auto end_frame() noexcept -> FrameResult;


				EVO_NODISCARD auto create_texture(
					const evo::ArrayProxy<byte> data, uint32_t width, uint32_t height, bool create_mipmaps
				) noexcept -> std::optional<uint64_t>; // texture ID


				EVO_NODISCARD auto create_mesh_3D(
					const evo::ArrayProxy<vulkan::Vertex3D> vbo, const evo::ArrayProxy<uint32_t> ibo
				) noexcept -> std::optional< std::pair<uint32_t, uint32_t> >; // vertex offset / index offset

				EVO_NODISCARD auto create_mesh_2D(
					const evo::ArrayProxy<vulkan::Vertex2D> vbo, const evo::ArrayProxy<uint32_t> ibo
				) noexcept -> std::optional< std::pair<uint32_t, uint32_t> >; // vertex offset / index offset


				EVO_NODISCARD auto resize(uint32_t width, uint32_t height) noexcept -> bool;


				auto set_global_ubo_3D(const void* data) noexcept -> void;
				auto set_instance_ubo_3D(uint32_t index, const void* data) noexcept -> void;

				auto set_global_ubo_2D(const void* data) noexcept -> void;
				auto set_instance_ubo_2D(uint32_t index, const void* data) noexcept -> void;

				// TODO: only does it for one in-flight frame
				auto set_instance_texture_3D(uint32_t descriptor_index, uint32_t texture_index) noexcept -> void;
				auto set_instance_texture_2D(uint32_t descriptor_index, uint32_t texture_index) noexcept -> void;

				auto begin_render_pass_3D() noexcept -> void;
				auto end_render_pass_3D() noexcept -> void;

				auto begin_render_pass_2D() noexcept -> void;
				auto end_render_pass_2D() noexcept -> void;

				auto allocate_descriptor_set_3D() noexcept -> std::optional<uint32_t>;
				auto allocate_descriptor_set_2D() noexcept -> std::optional<uint32_t>;



				// this is for instance level, global is done automatically
				auto bind_descriptor_set_3D(uint32_t index) noexcept -> void;
				auto bind_descriptor_set_2D(uint32_t index) noexcept -> void;

				auto set_model_push_constant_3D(const glm::mat4& model) noexcept -> void;
				auto set_model_push_constant_2D(const glm::mat4& model) noexcept -> void;

				auto bind_vertex_buffer_3D() noexcept -> void;
				auto bind_index_buffer_3D() noexcept -> void;
				auto bind_vertex_buffer_2D() noexcept -> void;
				auto bind_index_buffer_2D() noexcept -> void;
				auto draw_indexed(uint32_t index_count, uint32_t first_index, int32_t vertex_offset) noexcept -> void;


			private:

				EVO_NODISCARD auto create_render_targets() noexcept -> bool;
				auto destroy_render_targets() noexcept -> void;


				template<typename T>
				EVO_NODISCARD auto create_mesh_impl(
					const evo::ArrayProxy<T> vbo, const evo::ArrayProxy<uint32_t> ibo,
					vulkan::Buffer& vertex_buffer, uint32_t& vertex_buffer_index,
					vulkan::Buffer& index_buffer, uint32_t& index_buffer_index
				) noexcept -> std::optional< std::pair<uint32_t, uint32_t> >; // vertex offset / index offset



			private:
				VkInstance instance{VK_NULL_HANDLE};
				#if defined(PH_CONFIG_DEBUG)
					VkDebugUtilsMessengerEXT debug_messenger{VK_NULL_HANDLE};
				#endif
				VkSurfaceKHR surface{VK_NULL_HANDLE};

				VkSampleCountFlagBits msaa_samples = VK_SAMPLE_COUNT_1_BIT;
				static const bool use_sample_shading = false;
				vulkan::Device device{};


				vulkan::Swapchain swapchain{};
				std::vector<vulkan::Framebuffer> framebuffers_3D{};
				std::vector<vulkan::Framebuffer> framebuffers_2D{};

				vulkan::Image color_image{};
				vulkan::Image depth_image{};

				VkCommandPool command_pool{VK_NULL_HANDLE};
				std::array<vulkan::CommandBuffer, MAX_FRAMES_IN_FLIGHT> command_buffers{};

				std::array<vulkan::Semaphore, MAX_FRAMES_IN_FLIGHT> image_available_semaphores{};
				std::array<vulkan::Semaphore, MAX_FRAMES_IN_FLIGHT> render_finished_semaphores{};
				std::array<vulkan::Fence, MAX_FRAMES_IN_FLIGHT> in_flight_fences{};
				uint32_t current_frame = 0;


				std::vector<vulkan::Texture> textures{};



				///////////////////////////////////
				// 3D

				vulkan::Buffer vertex_buffer_3D{};
				uint32_t vertex_buffer_3D_index = 0;
				vulkan::Buffer index_buffer_3D{};
				uint32_t index_buffer_3D_index = 0;


				vulkan::RenderPassManager render_pass_3D{};
				vulkan::RenderPassManager::PipelineID pipeline_3D;

				vulkan::RenderPassManager::DescriptorSetLayoutID global_descriptor_set_layout_3D;
				vulkan::RenderPassManager::DescriptorSetLayoutID instance_descriptor_set_layout_3D;



				std::array<vulkan::Buffer, MAX_FRAMES_IN_FLIGHT> global_uniform_buffers_3D{};
				std::array<RenderPassManager::DescriptorSetID, MAX_FRAMES_IN_FLIGHT> global_descriptor_sets_3D{};

				std::vector< std::array<vulkan::Buffer                    , MAX_FRAMES_IN_FLIGHT> > instance_uniform_buffers_3D{};
				std::vector< std::array<RenderPassManager::DescriptorSetID, MAX_FRAMES_IN_FLIGHT> > instance_descriptor_sets_3D{};


				///////////////////////////////////
				// 2D


				vulkan::Buffer vertex_buffer_2D{};
				uint32_t vertex_buffer_2D_index = 0;
				vulkan::Buffer index_buffer_2D{};
				uint32_t index_buffer_2D_index = 0;



				vulkan::RenderPassManager render_pass_2D{};
				vulkan::RenderPassManager::PipelineID pipeline_2D;

				vulkan::RenderPassManager::DescriptorSetLayoutID global_descriptor_set_layout_2D;
				vulkan::RenderPassManager::DescriptorSetLayoutID instance_descriptor_set_layout_2D;


				std::array<vulkan::Buffer, MAX_FRAMES_IN_FLIGHT> global_uniform_buffers_2D{};
				std::array<RenderPassManager::DescriptorSetID, MAX_FRAMES_IN_FLIGHT> global_descriptor_sets_2D{};

				std::vector< std::array<vulkan::Buffer                    , MAX_FRAMES_IN_FLIGHT> > instance_uniform_buffers_2D{};
				std::vector< std::array<RenderPassManager::DescriptorSetID, MAX_FRAMES_IN_FLIGHT> > instance_descriptor_sets_2D{};
		};


		
	};
};

