#pragma once

#include "Event.h"


namespace Phoenix{


	class WindowCloseEvent : public Event{
		private:

		public:
			WindowCloseEvent(){}


			inline int getType() const override { return PH_WINDOW_CLOSE_EVENT; }


			std::string print() override {
				return "{ WindowCloseEvent }";
			}

	};

	class WindowResizeEvent : public Event{
		private:
			unsigned int width, height;

		public:
			WindowResizeEvent(unsigned int p_width, unsigned int p_height)
				: width(p_width), height(p_height) {
			}

			inline int getType() const override { return PH_WINDOW_RESIZE_EVENT; }

			inline unsigned int getWidth() const { return width; }
			inline unsigned int getHeight() const { return height; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ WindowResizeEvent (" << width << "x" << height << ") }";
				return ss.str();
			}

	};

	
}