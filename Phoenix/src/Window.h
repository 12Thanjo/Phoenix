#pragma once

#include "../ph_pch.h"
#include "events/event.h"

namespace Phoenix{

	struct WindowProperties{
		std::string title;
		unsigned int width, height;

		WindowProperties(const std::string& p_title = "Phoenix Engine", unsigned int p_width = 1280, unsigned int p_height = 720)
			: title(p_title), width(p_width), height(p_height) {

		}
	};



	class Window{
		private:
			
		public:
			using EventCallback = std::function<void(Event&)>;

			virtual void init(const WindowProperties& props) = 0;
			virtual void destroy() = 0;

			static Window* create(const WindowProperties& props = WindowProperties());

			virtual void set_vsync(bool enabled) = 0;
			virtual void run() = 0;

			virtual void set_callback(const EventCallback& callback) = 0;

			virtual void* getNativeWindow() const = 0;

			inline unsigned int getWidth() const { return data.width; }
			inline unsigned int getHeight() const { return data.height; }
			
		protected:
			WindowProperties data;
	};
}

