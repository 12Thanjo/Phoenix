#include "ph_pch.h"
#include "InputManager.h"



#include "Window.h"
#include "events/events.h"


namespace Phoenix{

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


	unsigned int static glfwKeycodeToPhoenix(int keycode){
		if(keycode == -1){
			return 0;
		}else{
			return glfw_keycodes[keycode];
		};
	};


	

	InputManager::InputManager(Window* window){
		for(int i = 0; i < sizeof(_keys_down); i++){
			_keys_down[i] = false;
		}

		for(int i = 0; i < sizeof(_mouse_buttons_down); i += 1){
			_mouse_buttons_down[i] = false;
		}


		// close
		glfwSetWindowCloseCallback(window->getWindowContext(), [](GLFWwindow* window_context){
			WindowConfig& data = *(WindowConfig*)glfwGetWindowUserPointer(window_context);

			WindowCloseEvent e{};

			// PH_LOG("Close");

			data.eventCallback(e, data.id);
		});

		// resize
		glfwSetWindowSizeCallback(window->getWindowContext(), [](GLFWwindow* window_context, int width, int height){
			WindowConfig& data = *(WindowConfig*)glfwGetWindowUserPointer(window_context);

			data.width = width;
			data.height = height;

			WindowResizeEvent e(width, height);

			// PH_LOG("resize");
			data.eventCallback(e, data.id);
		});

		// keyboard
		glfwSetKeyCallback(window->getWindowContext(), [](GLFWwindow* window_context, int key, int scancode, int action, int mods){
			WindowConfig& data = *(WindowConfig*)glfwGetWindowUserPointer(window_context);

			unsigned int ph_key = glfwKeycodeToPhoenix(key);

			if(action == GLFW_PRESS){
				KeyDownEvent e(ph_key);
				data.input_manager->_keys_down[ph_key] = true;
				data.eventCallback(e, data.id);
			}else if(action == GLFW_RELEASE){
				KeyUpEvent e(ph_key);
				data.input_manager->_keys_down[ph_key] = false;
				data.eventCallback(e, data.id);
			}

		});


		// mouse button
		glfwSetMouseButtonCallback(window->getWindowContext(), [](GLFWwindow* window_context, int button, int action, int mods){
			WindowConfig& data = *(WindowConfig*)glfwGetWindowUserPointer(window_context);


			if(action == GLFW_PRESS){
				MouseDownEvent e(button);
				data.input_manager->_mouse_buttons_down[button] = true;
				data.eventCallback(e, data.id);
			}else if(action == GLFW_RELEASE){
				MouseUpEvent e(button);
				data.input_manager->_mouse_buttons_down[button] = false;
				data.eventCallback(e, data.id);
			}
		});


		// mouse move
		glfwSetCursorPosCallback(window->getWindowContext(), [](GLFWwindow* window_context, double x, double y){
			WindowConfig& data = *(WindowConfig*)glfwGetWindowUserPointer(window_context);

			MouseMoveEvent e((float)x, (float)y);
			data.eventCallback(e, data.id);
		});


		// mouse scroll
		glfwSetScrollCallback(window->getWindowContext(), [](GLFWwindow* window_context, double x, double y){
			WindowConfig& data = *(WindowConfig*)glfwGetWindowUserPointer(window_context);

			MouseScrollEvent e((float)x, (float)y);
			data.eventCallback(e, data.id);
		});

	}


	
}
