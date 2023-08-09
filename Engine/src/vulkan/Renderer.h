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



// TODO: change this value
constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 1;
constexpr uint32_t MAX_DESCRIPTOR_SETS = 100; // this was just arbitrarily chosen


struct GlobalUBO3D{
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct GlobalUBO2D{
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};


struct InstanceUBO3D{
	glm::vec4 color;
};

struct InstanceUBO2D{
	glm::vec4 color;
};



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


				EVO_NODISCARD auto create_mesh(
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
				EVO_NODISCARD auto load_shader_code(const std::string& filepath) noexcept -> std::optional< std::vector<byte> >;


				EVO_NODISCARD auto create_render_targets() noexcept -> bool;
				auto destroy_render_targets() noexcept -> void;



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


				VkDescriptorPool descriptor_pool{VK_NULL_HANDLE};


				///////////////////////////////////
				// 3D

				vulkan::Buffer vertex_buffer_3D{};
				uint32_t vertex_buffer_3D_index = 0;
				vulkan::Buffer index_buffer_3D{};
				uint32_t index_buffer_3D_index = 0;


				vulkan::RenderPass render_pass_3D{};
				VkDescriptorSetLayout global_descriptor_set_layout_3D{VK_NULL_HANDLE};
				VkDescriptorSetLayout instance_descriptor_set_layout_3D{VK_NULL_HANDLE};
				VkPipelineLayout pipeline_layout_3D{VK_NULL_HANDLE};
				vulkan::Pipeline pipeline_3D{};


				std::array<vulkan::Buffer, MAX_FRAMES_IN_FLIGHT> global_uniform_buffers_3D{};
				std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> global_descriptor_sets_3D{};

				std::array<std::array<vulkan::Buffer, MAX_FRAMES_IN_FLIGHT>, MAX_DESCRIPTOR_SETS> instance_uniform_buffers_3D{};
				std::array<std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>, MAX_DESCRIPTOR_SETS> instance_descriptor_sets_3D{};


				///////////////////////////////////
				// 2D


				vulkan::Buffer vertex_buffer_2D{};
				uint32_t vertex_buffer_2D_index = 0;
				vulkan::Buffer index_buffer_2D{};
				uint32_t index_buffer_2D_index = 0;


				vulkan::RenderPass render_pass_2D{};
				VkDescriptorSetLayout global_descriptor_set_layout_2D{VK_NULL_HANDLE};
				VkDescriptorSetLayout instance_descriptor_set_layout_2D{VK_NULL_HANDLE};
				VkPipelineLayout pipeline_layout_2D{VK_NULL_HANDLE};
				vulkan::Pipeline pipeline_2D{};


				std::array<vulkan::Buffer, MAX_FRAMES_IN_FLIGHT> global_uniform_buffers_2D{};
				std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> global_descriptor_sets_2D{};

				std::array<std::array<vulkan::Buffer, MAX_FRAMES_IN_FLIGHT>, MAX_DESCRIPTOR_SETS> instance_uniform_buffers_2D{};
				std::array<std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT>, MAX_DESCRIPTOR_SETS> instance_descriptor_sets_2D{};
		};


		
	};
};

