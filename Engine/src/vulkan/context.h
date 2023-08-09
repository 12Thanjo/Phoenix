#pragma once

#include <libs/Vulkan/Vulkan.h>

#include <libs/evo/evo.h>

#include <optional>

namespace ph{
	namespace vulkan{



		EVO_NODISCARD auto create_instance(const std::string& app_name) noexcept -> std::optional<VkInstance>;
		auto destroy_instance(VkInstance& instance) noexcept -> void;


		#if defined(PH_CONFIG_DEBUG)
			EVO_NODISCARD auto create_debug_messenger(const VkInstance& instance) noexcept -> std::optional<VkDebugUtilsMessengerEXT>;
			auto destroy_debug_messenger(const VkInstance& instance, VkDebugUtilsMessengerEXT& debug_messenger) noexcept -> bool;
		#endif


		EVO_NODISCARD auto select_physical_device(const VkInstance& instance) noexcept -> std::optional<VkPhysicalDevice>;

		

		//////////////////////////////////////////////////////////////////////
		// logical device

		class Device{
			public:
				Device() = default;
				~Device() = default;

				EVO_NODISCARD auto create(const VkSurfaceKHR& surface, VkPhysicalDevice physical_device, bool use_sample_shading) noexcept -> bool;
				auto destroy() noexcept -> void;


				EVO_NODISCARD auto wait_idle() const noexcept -> bool;



				EVO_NODISCARD constexpr auto get_handle() const noexcept -> const VkDevice { return this->handle; };
				EVO_NODISCARD constexpr auto get_physical() const noexcept -> const VkPhysicalDevice { return this->physical; };

				EVO_NODISCARD constexpr auto get_graphics_queue() const noexcept -> const VkQueue { return this->graphics_queue; };
				EVO_NODISCARD constexpr auto get_present_queue() const noexcept -> const VkQueue { return this->present_queue; };
				EVO_NODISCARD constexpr auto get_transfer_queue() const noexcept -> const VkQueue { return this->transfer_queue; };
		
			private:
				VkDevice handle{VK_NULL_HANDLE};
				VkPhysicalDevice physical{VK_NULL_HANDLE};

				VkQueue graphics_queue{VK_NULL_HANDLE};
				VkQueue present_queue{VK_NULL_HANDLE};
				VkQueue transfer_queue{VK_NULL_HANDLE};
		};


	};
};