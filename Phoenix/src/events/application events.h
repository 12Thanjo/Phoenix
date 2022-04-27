#include "Event.h"


namespace Phoenix{

	class WindowResizeEvent : public Event{
		private:
			unsigned int width, height;

		public:
			WindowResizeEvent(unsigned int p_width, unsigned int p_height)
				: width(p_width), height(p_height) {

			}

			inline unsigned int getWidth() const { return width; }
			inline unsigned int getHeight() const { return height; }

	};
	
}