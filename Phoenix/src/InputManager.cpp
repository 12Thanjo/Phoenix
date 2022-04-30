#include "ph_pch.h"
#include "InputManager.h"

// #include "events/events.h"

namespace Phoenix{
	namespace InputManager{
		bool keys_down[120];

		void init(){
			for(int i = 0; i < 30; i++){
				keys_down[i] = false;
			}


			ph_internal_info("InputManager Initialized");
		};


		unsigned int glfw_keycodes[] = {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,62,0,0,0,0,0,0,63,
			0,0,0,0,64,65,66,67,10,1,2,3,4,5,6,7,8,9,0,68,
			0,69,0,0,0,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,
			26,27,28,29,30,31,32,33,34,35,36,70,71,72,0,0,73,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,74,75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,76,77,78,79,
			80,81,82,83,84,85,86,87,88,89,0,0,0,0,0,0,0,0,0,0,
			90,91,92,93,94,0,0,0,0,0,37,38,39,40,41,42,43,44,45,46,
			47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,0,0,0,0,0,
			95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,0,0,0,
			112,113,114,115,116,117,118,119
		};

		unsigned int glfwKeycodeToPhoenix(int keycode){
			if(keycode == -1){
				return 0;
			}else{
				return glfw_keycodes[keycode];
			};
		};


		void onEvent(Event& e){
			int type = e.getType();

			if(type == PH_KEY_DOWN_EVENT){
				keys_down[static_cast<KeyDownEvent&>(e).getKeycode()] = true;
			}else if(type == PH_KEY_UP_EVENT){
				keys_down[static_cast<KeyUpEvent&>(e).getKeycode()] = false;
			};
			
			// ph_internal_info(e);
		};

		bool keyDown(int keycode){
			return keys_down[keycode];
		}
	}
}