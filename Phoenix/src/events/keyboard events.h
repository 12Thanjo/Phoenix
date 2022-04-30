#pragma once


#include "Event.h"


namespace Phoenix{

	class KeyDownEvent : public Event{
		private:
			unsigned int keycode;

		public:
			KeyDownEvent(unsigned int p_keycode)
				: keycode(p_keycode) {
			}

			inline int getType() const override { return PH_KEY_DOWN_EVENT; }

			inline unsigned int getKeycode() const { return keycode; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ KeyDownEvent (" << keycode << ") }";
				return ss.str();
			}
	};


	class KeyUpEvent : public Event{
		private:
			unsigned int keycode;

		public:
			KeyUpEvent(unsigned int p_keycode)
				: keycode(p_keycode) {
			}

			inline int getType() const override { return PH_KEY_UP_EVENT; }

			inline unsigned int getKeycode() const { return keycode; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ KeyUpEvent (" << keycode << ") }";
				return ss.str();
			}
	};


	// class KeyTypeEvent : public Event{
	// 	private:
	// 		unsigned int keycode;

	// 	public:
	// 		KeyTypeEvent(unsigned int p_keycode)
	// 			: keycode(p_keycode) {
	// 		}

	// 		inline int getType() const override { return PH_KEY_TYPE_EVENT; }

	// 		inline unsigned int getKeycode() const { return keycode; }

	// 		std::string print() override {
	// 			std::stringstream ss;
	// 			ss << "{ KeyTypeEvent (" << keycode << ") }";
	// 			return ss.str();
	// 		}
	// };

		
}
