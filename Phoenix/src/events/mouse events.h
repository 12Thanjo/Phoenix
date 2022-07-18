#pragma once

#include "Event.h"

namespace Phoenix{
	
	class MouseDownEvent : public Event{
		public:
			MouseDownEvent(unsigned int p_button)
				: button(p_button) {
			}

			inline int getType() const override { return PH_MOUSE_DOWN_EVENT; }

			inline unsigned int getButton() const { return button; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ MouseDownEvent (" << button << ") }";
				return ss.str();
			}

		private:
			unsigned int button;

	};


	class MouseUpEvent : public Event{
		public:
			MouseUpEvent(unsigned int p_button)
				: button(p_button) {
			}

			inline int getType() const override { return PH_MOUSE_UP_EVENT; }

			inline unsigned int getButton() const { return button; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ MouseUpEvent (" << button << ") }";
				return ss.str();
			}

		private:
			unsigned int button;

	};


	class MouseMoveEvent : public Event{
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

		private:
			float x, y;

	};


	class MouseScrollEvent : public Event{
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

		private:
			float x, y;

	};

}


