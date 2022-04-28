#pragma once

#include "events/events.h"

namespace Phoenix{
	class Layer{
		public:
			Layer(const std::string& debug_name = "Layer");
			virtual ~Layer();

			virtual void onEvent(Event& event){};
			virtual void onUpdate(){};


			inline const std::string& getName() const { return name; }

		protected:
			std::string name;
	};
}


