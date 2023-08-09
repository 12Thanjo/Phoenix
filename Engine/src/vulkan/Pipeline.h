#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>


#include <optional>

namespace ph{
	namespace vulkan{
		
		EVO_NODISCARD auto create_shader_module(
			const class Device& device, const evo::ArrayProxy<byte> code
		) noexcept -> std::optional<VkShaderModule>;
		
		auto destroy_shader_module(const class Device& device, VkShaderModule& shader_module) noexcept -> void;
		

		EVO_NODISCARD auto create_pipeline_layout(
			const Device& device,
			const evo::ArrayProxy<VkDescriptorSetLayout> descriptor_set_layouts,
			const evo::ArrayProxy<VkPushConstantRange> push_constant_ranges
		) noexcept -> std::optional<VkPipelineLayout>;

		auto destroy_pipeline_layout(const class Device& device, VkPipelineLayout& pipeline_layout) noexcept -> void;



		//////////////////////////////////////////////////////////////////////
		// descriptors

		EVO_NODISCARD auto create_descriptor_set_layout(
			const class Device& device, evo::ArrayProxy<VkDescriptorSetLayoutBinding> bindings
		) noexcept -> std::optional<VkDescriptorSetLayout>;

		EVO_NODISCARD auto destroy_descriptor_set_layout(const class Device& device, VkDescriptorSetLayout& descriptor_set_layout) noexcept -> void;




		EVO_NODISCARD auto create_descriptor_pool(
			const class Device& device, uint32_t max_sets, evo::ArrayProxy<VkDescriptorPoolSize> pool_sizes
		) noexcept -> std::optional<VkDescriptorPool>;

		EVO_NODISCARD auto destroy_descriptor_pool(const class Device& device, VkDescriptorPool& descriptor_pool) noexcept -> void;



		EVO_NODISCARD auto allocate_descriptor_sets(
			const class Device& device, const VkDescriptorPool& pool, const evo::ArrayProxy<VkDescriptorSetLayout> layouts
		) noexcept -> std::optional< std::vector<VkDescriptorSet> >;




		//////////////////////////////////////////////////////////////////////
		// pipeline

		struct PipelineConfig{
			VkSampleCountFlagBits msaa_samples       = VK_SAMPLE_COUNT_1_BIT;
			bool                  use_sample_shading = false;



			auto add_vertex_shader(const VkShaderModule& vertex_shader) noexcept -> void;
			auto add_fragment_shader(const VkShaderModule& fragment_shader) noexcept -> void;

			auto add_vertex_binding(uint32_t size, VkVertexInputRate input_rate) noexcept -> void;
			auto add_vertex_attribute(VkFormat format, uint32_t binding = 0) noexcept -> void;


			///////////////////////////////////
			// internal

			std::vector<VkPipelineShaderStageCreateInfo> shader_stages{};
			std::vector<VkVertexInputBindingDescription> vertex_bindings{};
			std::vector<VkVertexInputAttributeDescription> vertex_attributes{};
		};




		class Pipeline{
			public:
				Pipeline() = default;
				~Pipeline() = default;

				EVO_NODISCARD auto create(
					const class Device& device,
					const class RenderPass& render_pass,
					const VkPipelineLayout& pipeline_layout,
					const PipelineConfig& config,
					const VkPipelineCache& pipeline_cache = VK_NULL_HANDLE
				) noexcept -> bool;
				auto destroy(const class Device& device) noexcept -> void;

			
			// TODO: private
			public:
				VkPipeline handle{VK_NULL_HANDLE};
		};


	};
};