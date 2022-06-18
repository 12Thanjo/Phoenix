#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InputManager.h"

#include "events/Event.h"

namespace Phoenix{

	struct WindowConfig{
		int width;
		int height;
		std::string name = "Phoenix Engine";
		std::function<void(Event&)> eventCallback;

		bool VSync = false;
		bool captureMouse = false;

		int GLFWVersionMajor = 4;
		int GLFWVersionMinor = 6;

		// internal
		InputManager* input_manager;
	};

	
	class Window{
		private:
			GLFWwindow* _window;
			WindowConfig _config;
	
		public:
			Window(WindowConfig config);
			~Window();

			void render();
			void pollEvents();


			inline GLFWwindow* getWindowContext() const { return _window; }
			inline int getWidth() const { return _config.width; }
			inline int getHeight() const { return _config.height; }


			void bind();

			inline bool keyDown(keyCode key) const { return _config.input_manager->keyDown(key); }
			inline bool mouseButtonDown(keyCode key) const { return _config.input_manager->mouseButtonDown(key); }
			inline float mouseX() const { return _config.input_manager->mouseX(); };
			inline float mouseY() const { return _config.input_manager->mouseY(); };


			// setting ////////////
			void setVSync(bool use);
			void captureMouse(bool capture);
	};

}
