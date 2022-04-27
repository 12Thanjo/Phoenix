


#pragma once

#include "Window.h"
#include <GLFW/glfw3.h>

namespace Phoenix{
	class WindowsWindow : public Window{
		private:
			GLFWwindow* window;

		public:
			WindowsWindow(const WindowProperties& props);
			~WindowsWindow();

			void init(const WindowProperties& props) override;
			void destroy() override;


			void set_vsync(bool enabled) override;
			void run() override;

			inline void set_callback(const EventCallback& callback) override { data.event_callback = callback; }


			struct WindowData {
				std::string title;
				unsigned int width, height;
				bool VSync;
				EventCallback event_callback;
			};


			WindowData data;
	};
}
