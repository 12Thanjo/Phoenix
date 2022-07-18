#pragma once

#include <Phoenix.h>

#define GRAB_EVENT   8
#define ROTATE_EVENT 9
#define SCALE_EVENT  10

namespace Phoenix{

	class GrabEvent : public Event{
		public:
			GrabEvent(){};

			inline EventType getType() const override { return GRAB_EVENT; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ GRAB_EVENT }";
				return ss.str();
			}

	};


	class RotateEvent : public Event{
		public:
			RotateEvent(){};

			inline EventType getType() const override { return ROTATE_EVENT; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ ROTATE_EVENT }";
				return ss.str();
			}

	};


	class ScaleEvent : public Event{
		public:
			ScaleEvent(){};

			inline EventType getType() const override { return SCALE_EVENT; }

			std::string print() override {
				std::stringstream ss;
				ss << "{ SCALE_EVENT }";
				return ss.str();
			}

	};

}