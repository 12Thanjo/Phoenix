#include "Pipeline.h"

#include "Logging.h"


#include "vulkan/utils.h"
#include "vulkan/context.h"
#include "vulkan/RenderPass.h"


#include <format>


namespace ph{
	namespace vulkan{


		auto create_shader_module(const Device& device, const evo::ArrayProxy<byte> code) noexcept -> std::optional<VkShaderModule> {
			const auto create_info = VkShaderModuleCreateInfo{ .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				.codeSize = code.size(),
				.pCode    = reinterpret_cast<const uint32_t*>(code.data()),
			};

			VkShaderModule shader_module;
			const VkResult result = vkCreateShaderModule(device.get_handle(), &create_info, nullptr, &shader_module);
			if(vulkan::result_fail(result, "vkCreateShaderModule")) return std::nullopt;


			PH_TRACE("Created: Vulkan shader module");
			return shader_module;
		};



		auto destroy_shader_module(const Device& device, VkShaderModule& shader_module) noexcept -> void {
			PH_ASSERT(shader_module != VK_NULL_HANDLE, "Shader module was already destroyed or was never created");

			vkDestroyShaderModule(device.get_handle(), shader_module, nullptr);
			shader_module = VK_NULL_HANDLE;

			PH_TRACE("Destroyed: Vulkan shader module");
		};






		auto create_pipeline_layout(
			const Device& device,
			const evo::ArrayProxy<VkDescriptorSetLayout> descriptor_set_layouts,
			const evo::ArrayProxy<VkPushConstantRange> push_constant_ranges
		) noexcept -> std::optional<VkPipelineLayout> {


			const auto create_info = VkPipelineLayoutCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
				.setLayoutCount         = static_cast<uint32_t>(descriptor_set_layouts.size()),
				.pSetLayouts            = descriptor_set_layouts.data(),

				.pushConstantRangeCount = static_cast<uint32_t>(push_constant_ranges.size()),
				.pPushConstantRanges    = push_constant_ranges.data(),
			};

			VkPipelineLayout pipeline_layout;
			const VkResult result = vkCreatePipelineLayout(device.get_handle(), &create_info, nullptr, &pipeline_layout);
			if(vulkan::result_fail(result, "vkCreatePipelineLayout")) return std::nullopt;

			PH_DEBUG("Created: Vulkan pipeline layout");
			return std::move(pipeline_layout);
		};



		auto destroy_pipeline_layout(const Device& device, VkPipelineLayout& pipeline_layout) noexcept -> void {
			PH_ASSERT(pipeline_layout != VK_NULL_HANDLE, "Pipeline was already destroyed or was never created");

			vkDestroyPipelineLayout(device.get_handle(), pipeline_layout, nullptr);
			pipeline_layout = VK_NULL_HANDLE;

			PH_TRACE("Destroyed: Vulkan pipeline layout");
		};



		//////////////////////////////////////////////////////////////////////
		// descriptors

		auto create_descriptor_set_layout(
			const Device& device, evo::ArrayProxy<VkDescriptorSetLayoutBinding> bindings
		) noexcept -> std::optional<VkDescriptorSetLayout> {

			const auto create_info = VkDescriptorSetLayoutCreateInfo{ .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
				.bindingCount = static_cast<uint32_t>(bindings.size()),
				.pBindings    = bindings.data(),
			};


			VkDescriptorSetLayout layout;
			const VkResult result = vkCreateDescriptorSetLayout(device.get_handle(), &create_info, nullptr, &layout);
			if(vulkan::result_fail(result, "vkCreateDescriptorSetLayout")) return std::nullopt;

			PH_TRACE("Created Vulkan descriptor set layout");
			return std::move(layout);
		};



		auto destroy_descriptor_set_layout(const Device& device, VkDescriptorSetLayout& descriptor_set_layout) noexcept -> void {
			vkDestroyDescriptorSetLayout(device.get_handle(), descriptor_set_layout, nullptr);
			descriptor_set_layout = VK_NULL_HANDLE;

			PH_TRACE("Destroyed: Vulkan descriptor set layout");
		};







		auto create_descriptor_pool(
			const Device& device, uint32_t max_sets, evo::ArrayProxy<VkDescriptorPoolSize> pool_sizes
		) noexcept -> std::optional<VkDescriptorPool> {


			auto create_info = VkDescriptorPoolCreateInfo{ .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
				.maxSets       = max_sets,
				.poolSizeCount = static_cast<uint32_t>(pool_sizes.size()),
				.pPoolSizes    = pool_sizes.data(),
			};



			VkDescriptorPool descriptor_pool;
			const VkResult result = vkCreateDescriptorPool(device.get_handle(), &create_info, nullptr, &descriptor_pool);
			if(vulkan::result_fail(result, "vkCreateDescriptorPool")) return std::nullopt;


			PH_TRACE("Created: Vulkan descriptor pool");
			return std::move(descriptor_pool);
		};



		auto destroy_descriptor_pool(const Device& device, VkDescriptorPool& descriptor_pool) noexcept -> void {
			PH_ASSERT(descriptor_pool != VK_NULL_HANDLE, "Descriptor pool was already destroyed or was never created");

			vkDestroyDescriptorPool(device.get_handle(), descriptor_pool, nullptr);
			descriptor_pool = VK_NULL_HANDLE;

			PH_TRACE("Destroyed: Vulkan descriptor pool");
		};




		auto allocate_descriptor_sets(
			const Device& device, const VkDescriptorPool& pool, const evo::ArrayProxy<VkDescriptorSetLayout> layouts
		) noexcept -> std::optional< std::vector<VkDescriptorSet> > {

			const auto alloc_info = VkDescriptorSetAllocateInfo{ .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
				.descriptorPool     = pool,
				.descriptorSetCount = static_cast<uint32_t>(layouts.size()),
				.pSetLayouts        = layouts.data(),
			};


			auto descriptor_sets = std::vector<VkDescriptorSet>(layouts.size());

			const VkResult result = vkAllocateDescriptorSets(device.get_handle(), &alloc_info, descriptor_sets.data());
			if(vulkan::result_fail(result, "vkAllocateDescriptorSets")) return std::nullopt;


			PH_TRACE(std::format("Allocated: Vulkan descriptor sets ({})", layouts.size()));
			return std::move(descriptor_sets);
		};


		//////////////////////////////////////////////////////////////////////
		// pipeline config


		auto PipelineConfig::add_vertex_shader(const VkShaderModule& vertex_shader) noexcept -> void {
			this->shader_stages.emplace_back() = VkPipelineShaderStageCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_VERTEX_BIT,
				.module = vertex_shader,
				.pName = "main",
			};
		};


		auto PipelineConfig::add_fragment_shader(const VkShaderModule& fragment_shader) noexcept -> void {
			this->shader_stages.emplace_back() = VkPipelineShaderStageCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
				.module = fragment_shader,
				.pName = "main",
			};
		};


		auto PipelineConfig::add_vertex_binding(uint32_t size, VkVertexInputRate input_rate) noexcept -> void {
			const uint32_t binding = static_cast<uint32_t>(this->vertex_bindings.size());
			this->vertex_bindings.emplace_back(binding, size, input_rate);
		};


		auto PipelineConfig::add_vertex_attribute(VkFormat format, uint32_t binding) noexcept -> void {
			const uint32_t location = static_cast<uint32_t>(this->vertex_attributes.size());
			const uint32_t offset = [&](){
				if(this->vertex_attributes.empty()) return uint32_t(0);

				const auto& last_added = this->vertex_attributes[this->vertex_attributes.size() - 1];
				return last_added.offset + vulkan::format_size(last_added.format);
			}();

			this->vertex_attributes.emplace_back(location, binding, format, offset);
		};



		//////////////////////////////////////////////////////////////////////
		// pipeline


		auto Pipeline::create(
			const Device& device,
			const vulkan::RenderPass& render_pass,
			const VkPipelineLayout& pipeline_layout,
			const PipelineConfig& config,
			const VkPipelineCache& pipeline_cache
		) noexcept -> bool {


			const auto vertex_input = VkPipelineVertexInputStateCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
				.vertexBindingDescriptionCount   = static_cast<uint32_t>(config.vertex_bindings.size()),
				.pVertexBindingDescriptions      = config.vertex_bindings.data(),

				.vertexAttributeDescriptionCount = static_cast<uint32_t>(config.vertex_attributes.size()),
				.pVertexAttributeDescriptions    = config.vertex_attributes.data(),
			};




			const auto input_assembly = VkPipelineInputAssemblyStateCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.primitiveRestartEnable = VK_FALSE,
			};

			const auto viewport_state = VkPipelineViewportStateCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
				.viewportCount = 1,
				.pViewports    = nullptr,
				.scissorCount  = 1,
				.pScissors     = nullptr,
			};

			const auto rasterization_state = VkPipelineRasterizationStateCreateInfo{.sType=VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.depthClampEnable        = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode             = VK_POLYGON_MODE_FILL,
				.cullMode                = VK_CULL_MODE_BACK_BIT,
				.frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
				.depthBiasEnable         = VK_FALSE,
				.depthBiasConstantFactor = 0.0f,
				.depthBiasClamp          = 0.0f,
				.depthBiasSlopeFactor    = 0.0f,
				.lineWidth               = 1.0f,
			};

			const auto multisample_state = VkPipelineMultisampleStateCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.rasterizationSamples  = config.msaa_samples,
				.sampleShadingEnable   = config.use_sample_shading ? VK_TRUE : VK_FALSE,
				.minSampleShading      = config.use_sample_shading ? 0.8f    : 1.0f, // not sure 0.8 is the best value
				.pSampleMask           = nullptr,
				.alphaToCoverageEnable = VK_FALSE,
				.alphaToOneEnable      = VK_FALSE,
			};

			const auto depth_stencil_state = VkPipelineDepthStencilStateCreateInfo{ .sType=VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
				.depthTestEnable       = VK_TRUE,
				.depthWriteEnable      = VK_TRUE,
				.depthCompareOp        = VK_COMPARE_OP_LESS,
				.depthBoundsTestEnable = VK_FALSE,
				.stencilTestEnable     = VK_FALSE,
			};

			const auto color_attachment_state = VkPipelineColorBlendAttachmentState{
				.blendEnable         = VK_TRUE,
				.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
				.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				.colorBlendOp        = VK_BLEND_OP_ADD,
				.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
				.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
				.alphaBlendOp        = VK_BLEND_OP_ADD,
				.colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
			};

			const auto color_blend_state = VkPipelineColorBlendStateCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.logicOpEnable     = VK_FALSE,
				.logicOp           = VK_LOGIC_OP_COPY,
				.attachmentCount   = 1,
				.pAttachments      = &color_attachment_state,
				.blendConstants    = {0.0f, 0.0f, 0.0f, 0.0f},
			};

			const auto dynamic_states = std::to_array<VkDynamicState>({
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			});

			const auto dynamic_state = VkPipelineDynamicStateCreateInfo{ .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
				.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
				.pDynamicStates    = dynamic_states.data(),
			};

			

			const auto create_info = VkGraphicsPipelineCreateInfo{ .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
				.stageCount          = static_cast<uint32_t>(config.shader_stages.size()),
				.pStages             = config.shader_stages.data(),
				.pVertexInputState   = &vertex_input,
				.pInputAssemblyState = &input_assembly,
				.pViewportState      = &viewport_state,
				.pRasterizationState = &rasterization_state,
				.pMultisampleState   = &multisample_state,
				.pDepthStencilState  = &depth_stencil_state,
				.pColorBlendState    = &color_blend_state,
				.pDynamicState       = &dynamic_state,
				.layout              = pipeline_layout,
				.renderPass          = render_pass.handle,

				.subpass             = 0,
				.basePipelineHandle  = VK_NULL_HANDLE,
				.basePipelineIndex   = -1,
			};


			const VkResult result = vkCreateGraphicsPipelines(device.get_handle(), pipeline_cache, 1, &create_info, nullptr, &this->handle);
			if(vulkan::result_fail(result, "vkCreateGraphicsPipelines")) return false;


			PH_DEBUG("Created: Vulkan pipeline");
			return true;
		};



		auto Pipeline::destroy(const Device& device) noexcept -> void {
			vkDestroyPipeline(device.get_handle(), this->handle, nullptr);
			this->handle = VK_NULL_HANDLE;

			PH_DEBUG("Destroyed: Vulkan pipeline");
		};
		
		
	};
};