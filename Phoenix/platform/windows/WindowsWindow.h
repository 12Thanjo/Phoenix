


#pragma once

#include "Window.h"
#include "renderer/RenderContext.h"

#include <GLFW/glfw3.h>

namespace Phoenix{
	class WindowsWindow : public Window{
		private:
			GLFWwindow* window;
			RenderContext* context;

		public:
			WindowsWindow(const WindowProperties& props);
			~WindowsWindow();

			void init(const WindowProperties& props) override;
			void destroy() override;


			void set_vsync(bool enabled) override;
			void run() override;

			inline void set_callback(const EventCallback& callback) override { data.event_callback = callback; }
			inline virtual void* getNativeWindow() const { return window; }


			struct WindowData {
				std::string title;
				unsigned int width, height;
				bool VSync;
				EventCallback event_callback;
			};


			WindowData data;
	};
}
