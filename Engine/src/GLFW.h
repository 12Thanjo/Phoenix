#pragma once


#include <libs/evo/evo.h>

#include <functional>

struct GLFWwindow;


namespace ph{
	namespace GLFW{

		
		EVO_NODISCARD auto init() noexcept -> bool;
		auto shutdown() noexcept -> void;

		auto poll_events() noexcept -> void;
		auto wait_events() noexcept -> void;

		#if defined(VULKAN_H_)
			EVO_NODISCARD auto get_required_instance_extensions() noexcept -> std::vector<const char*>;
		#endif



		class Window{
			public:
				Window() = default;
				~Window() = default;

				EVO_NODISCARD auto create(uint32_t width, uint32_t height, const std::string& title) noexcept -> bool;
				auto destroy() noexcept -> void;

				#if defined(PH_PLATFORM_WINDOWS)
					auto use_darkmode() noexcept -> bool;
				#endif


				EVO_NODISCARD auto size() const noexcept -> std::pair<int, int>;


				auto set_resize_callback(std::function<void(int, int)> callback) noexcept -> void;
				auto set_mouse_button_callback(std::function<void(int, int, int)> callback) noexcept -> void;
				auto set_key_callback(std::function<void(int, int, int, int)> callback) noexcept -> void;
				auto set_mouse_move_callback(std::function<void(double, double)> callback) noexcept -> void;


				auto set_title(const std::string& title) noexcept -> void;

				auto capture_mouse() noexcept -> void;
				auto release_mouse() noexcept -> void;



				EVO_NODISCARD constexpr auto initialized() const noexcept -> bool { return this->handle == nullptr; };

				// TODO: temp
				EVO_NODISCARD auto should_close() const noexcept -> bool;



				///////////////////////////////////
				// vulkan
				#if defined(VULKAN_H_)
					auto create_surface(const VkInstance& instance) noexcept -> std::optional<VkSurfaceKHR>;
				#endif
		
			private:
				GLFWwindow* handle = nullptr;
		};


	};
};