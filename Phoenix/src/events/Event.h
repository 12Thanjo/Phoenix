#pragma once

#define PH_WINDOW_CLOSE_EVENT 0
#define PH_WINDOW_RESIZE_EVENT 1
#define PH_KEY_DOWN_EVENT 2
#define PH_KEY_UP_EVENT 3
#define PH_KEY_TYPE_EVENT 3
#define PH_MOUSE_DOWN_EVENT 4
#define PH_MOUSE_UP_EVENT 5
#define PH_MOUSE_MOVE_EVENT 6
#define PH_MOUSE_SCROLL_EVENT 7



namespace Phoenix{
	
	class Event{
		private:
	
		public:
			bool handled = false;

			virtual inline int getType() const { return -1; };

			virtual std::string print(){
				return "{ Event }";
			}


	};


	static std::ostream& operator << (std::ostream& os, Event& event){
		return os << event.print();
	}



}