#include <libs/GLFW/GLFW.h>
#include <libs/Vulkan/Vulkan.h>

#include "GLFW.h"


#include "vulkan/utils.h"

#include "Logging.h"
#include <format>


#if defined(PH_PLATFORM_WINDOWS)

	// #undef UNICODE
	#include <dwmapi.h>
	#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
		#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
	#endif

#endif


namespace ph{
	namespace GLFW{


		#if defined(PH_CONFIG_DEBUG)
			static bool glfw_initialized = false;
		#endif


		static auto glfw_error_callback(int error, const char* description) noexcept -> void {
			PH_ERROR( std::format("GLFW Error: (code: {}) - '{}'", error, description) );
		};


		//////////////////////////////////////////////////////////////////////
		// functions

		auto init() noexcept -> bool {
			#if defined(PH_CONFIG_DEBUG)
				PH_ASSERT(glfw_initialized == false, "GLFW already intialized");
				glfw_initialized = true;
			#endif

			int return_code = glfwInit();
			bool success = (return_code == GLFW_TRUE);

			if(success == false){
				PH_ERROR("Failed to initialze GLFW");
				return false;
			}

			glfwSetErrorCallback(glfw_error_callback);

			PH_DEBUG("Initialized: GLFW");
			return true;
		};


		auto shutdown() noexcept -> void {
			#if defined(PH_CONFIG_DEBUG)
				PH_ASSERT(glfw_initialized, "GLFW not intialized (attempted to shutdown)");
				glfw_initialized = false;
			#endif

			glfwTerminate();
			PH_DEBUG("Shutdown: GLFW");
		};


		auto poll_events() noexcept -> void {
			glfwPollEvents();
		};

		auto wait_events() noexcept -> void {
			glfwWaitEvents();
		};


		auto get_required_instance_extensions() noexcept -> std::vector<const char*> {
			uint32_t count = 0;
			const char** list;
			list = glfwGetRequiredInstanceExtensions(&count);

			return std::vector<const char*>(list, list + count);
		};




		//////////////////////////////////////////////////////////////////////
		// window

		auto Window::create(uint32_t width, uint32_t height, const std::string& title) noexcept -> bool {
			PH_ASSERT(this->handle == nullptr, "Window already created");


			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
																		   // fullscreen
			this->handle = glfwCreateWindow(width, height, title.c_str(), /*glfwGetPrimaryMonitor()*/ nullptr, nullptr);

			// {
			// 	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			// 	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			// 	glfwSetWindowMonitor(this->handle, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			// }



			if(this->handle == nullptr){
				const char* description;
				int code = glfwGetError(&description);
				PH_ERROR(std::format("GLFW Error({}): '{}'", code, description));

				return false;
			}

			// Fixes a bug where when drag resizing the window and the height becomes 0,
			// 	GLFW no longer recieves input events. This is probably an issue with
			// 	Windows, not GLFW.
			glfwSetWindowSizeLimits(this->handle, 1, 1, GLFW_DONT_CARE, GLFW_DONT_CARE);


			PH_DEBUG("Created: GLFW Window");
			return true;
		};



		auto Window::destroy() noexcept -> void {
			PH_ASSERT(this->handle != nullptr, "Window not created");

			glfwDestroyWindow(this->handle);

			this->handle = nullptr;
			PH_DEBUG("Destroyed: GLFW Window");
		};


		#if defined(PH_PLATFORM_WINDOWS)
			auto Window::use_darkmode() noexcept -> bool {
				glfwHideWindow(this->handle);

				BOOL darkmode_success = TRUE;
				auto hwnd = glfwGetWin32Window(this->handle);
				::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkmode_success, sizeof(darkmode_success));

				glfwShowWindow(this->handle);

				return static_cast<bool>(darkmode_success);
			};
		#endif






		auto Window::set_resize_callback(std::function<void(int, int)> callback) noexcept -> void {
			PH_ASSERT(this->handle != nullptr, "Window not created");

			const static auto callback_func = callback;

			glfwSetFramebufferSizeCallback(this->handle, []([[maybe_unused]] GLFWwindow* window_handle, int width, int height){
				callback_func(width, height);
			});
		};


		auto Window::set_mouse_button_callback(std::function<void(int, int, int)> callback) noexcept -> void {
			PH_ASSERT(this->handle != nullptr, "Window not created");

			const static auto callback_func = callback;

			glfwSetMouseButtonCallback(this->handle, []([[maybe_unused]] GLFWwindow* window_handle, int button, int action, int mods){
				callback_func(button, action, mods);
			});
		};


		auto Window::set_key_callback(std::function<void(int, int, int, int)> callback) noexcept -> void {
			PH_ASSERT(this->handle != nullptr, "Window not created");

			const static auto callback_func = callback;

			glfwSetKeyCallback(this->handle, []([[maybe_unused]] GLFWwindow* window_handle, int keycode, int scancode, int action, int mods){
				callback_func(keycode, scancode, action, mods);
			});
		};


		auto Window::set_mouse_move_callback(std::function<void(double, double)> callback) noexcept -> void {
			PH_ASSERT(this->handle != nullptr, "Window not created");

			const static auto callback_func = callback;

			glfwSetCursorPosCallback(this->handle, []([[maybe_unused]] GLFWwindow* window_handle, double x, double y){
				callback_func(x, y);
			});
		};



		auto Window::set_title(const std::string& title) noexcept -> void {
			glfwSetWindowTitle(this->handle, title.c_str());
		};

		auto Window::capture_mouse() noexcept -> void {
			glfwSetInputMode(this->handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		};

		auto Window::release_mouse() noexcept -> void {
			glfwSetInputMode(this->handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		};





		auto Window::should_close() const noexcept -> bool {
			return glfwWindowShouldClose(this->handle);
		};


		auto Window::size() const noexcept -> std::pair<int, int> {
			PH_ASSERT(this->handle != nullptr, "Window not created");

			auto output = std::pair<int, int>{};
			glfwGetWindowSize(this->handle, &output.first, &output.second);
			return output;
		};



		auto Window::create_surface(const VkInstance& instance) noexcept -> std::optional<VkSurfaceKHR> {
			auto surface = VkSurfaceKHR{VK_NULL_HANDLE};

			const VkResult result = glfwCreateWindowSurface(instance, this->handle, nullptr, &surface);

			if(vulkan::result_fail(result, "glfwCreateWindowSurface")){
				const char* description;
				int code = glfwGetError(&description);
				PH_ERROR(std::format("GLFW Error({}): '{}'", code, description));

				return std::nullopt;
			}

			PH_DEBUG("Created: Vulkan Surface");
			return surface;
		}


		
	};
};