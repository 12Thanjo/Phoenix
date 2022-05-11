#pragma once

#include <GLFW/glfw3.h>

#include "events/events.h"
#include "FrameBuffer.h"

namespace Phoenix{

	struct WindowProperties{
		std::string name;
		uint32_t width, height;
		bool use_frame_buffer;

		WindowProperties(const std::string& _name = "Phoenix Engine", uint32_t _width = 1280, uint32_t _height = 720, bool _use_frame_buffer = false)
			: name(_name), width(_width), height(_height), use_frame_buffer(_use_frame_buffer) {

		}
	};


	class Window{
		private:
			GLFWwindow* window_context;

			struct WindowData {
				std::string name;
				unsigned int width, height;
				bool vsync;
				std::function<void(Event&)> event_callback;
				bool use_frame_buffer;
			};
			WindowData data;
			
			Ref<FrameBuffer> frame_buffer;

		public:
			Window(const WindowProperties& props);
			~Window();


			void clear();
			void update();

			void resize();


			void set_vsync(bool vsync);

			void set_event_callback(const std::function<void(Event&)>& callback){
				data.event_callback = callback;
			}


			inline unsigned int get_width() const { return data.width; }
			inline unsigned int get_height() const { return data.height; }


			inline GLFWwindow* get_native_window() const { return window_context; }
			inline RendererID get_frame_buffer_color_attachment() const { return frame_buffer->get_color_attachment(); }
			void resize_frame_buffer(uint32_t width, uint32_t height){ frame_buffer->resize(width, height); }
	};

}
