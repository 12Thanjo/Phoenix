#pragma once

#include "Event.h"

namespace Phoenix{
	
	class MouseDownEvent : public Event{
		private:
			unsigned int button;

		public:
			MouseDownEvent(unsigned int p_button)
				: button(p_button) {
			}

			inline int getType() const override { return PH_MOUSE_DOWN_EVENT; }

			inline unsigned int getKeycode() const { return button; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ MouseDownEvent (" << button << ") }";
				return ss.str();
			}
	};


	class MouseUpEvent : public Event{
		private:
			unsigned int button;

		public:
			MouseUpEvent(unsigned int p_button)
				: button(p_button) {
			}

			inline int getType() const override { return PH_MOUSE_UP_EVENT; }

			inline unsigned int getKeycode() const { return button; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ MouseUpEvent (" << button << ") }";
				return ss.str();
			}
	};


	class MouseMoveEvent : public Event{
		private:
			float x, y;

		public:
			MouseMoveEvent(float p_x, float p_y)
				: x(p_x), y(p_y) {
			}

			inline int getType() const override { return PH_MOUSE_MOVE_EVENT; }

			inline float getX() const { return x; }
			inline float getY() const { return y; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ MouseMoveEvent (" << x << ", " << y << ") }";
				return ss.str();
			}

	};


	class MouseScrollEvent : public Event{
		private:
			float x, y;

		public:
			MouseScrollEvent(float p_x, float p_y)
				: x(p_x), y(p_y) {
			}

			inline int getType() const override { return PH_MOUSE_SCROLL_EVENT; }

			inline float getX() const { return x; }
			inline float getY() const { return y; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ MouseScrollEvent (" << x << ", " << y << ") }";
				return ss.str();
			}

	};

}


