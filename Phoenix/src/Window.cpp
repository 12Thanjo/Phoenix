#include "ph_pch.h"
#include "Window.h"

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>


namespace Phoenix{

	static bool GLFW_initialized = false;
	static void glfw_error_callback(int error, const char* description){
		PH_ERROR("GLFW Error: (" << error << ")\n\t" << description);
	};

	
	Window::Window(WindowConfig config)
		: _config(config){

		PH_ASSERT(config.eventCallback, "Must define eventCallback in WindowConfig");

		// enforcing internals
		PH_ASSERT(config.input_manager, "window InputManager should not be set");




		if(!GLFW_initialized){
			if(!glfwInit()){
				PH_FATAL("GLFW Initialization Failed");
			}
			glfwSetErrorCallback(glfw_error_callback);
			
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _config.GLFWVersionMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _config.GLFWVersionMinor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			GLFW_initialized = true;
		}


		_window = glfwCreateWindow(_config.width, _config.height, _config.name.c_str(), nullptr, nullptr);
		// GLFWwindow* window = glfwCreateWindow((int)WINDOW_WIDTH, (int)WINDOW_HEIGHT, 
			// "Phoenix", glfwGetPrimaryMonitor(), nullptr);
		// glfwSetWindowAttrib(window_context, GLFW_DECORATED, GLFW_FALSE);
		// glfwSetWindowPos(window_context, 0, 0);
		// glfwSetWindowSize(window_context, 1920, 1080);
		if(!_window){
			PH_FATAL("Failed to create GLFW window");
			glfwTerminate();
		}

		glfwMakeContextCurrent(_window);

		// initialize GLAD
		// if(!GLFW_initialized){
			if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
				PH_FATAL("Failed to initialize GLAD");
				glfwTerminate();
			}

			PH_INFO("OpenGL: \n\tVendor:   " << glGetString(GL_VENDOR)   << 
							"\n\tRenderer: " << glGetString(GL_RENDERER) << 
							"\n\tDriver:   " << glGetString(GL_VERSION)  <<
							"\n\tShader:   " << glGetString(GL_SHADING_LANGUAGE_VERSION));


		// }


		glfwSetWindowUserPointer(_window, &_config);
		_config.input_manager = new InputManager(this);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);


		setVSync(_config.VSync);
		captureMouse(_config.captureMouse);



		PH_INFO("Created Window (" << _config.name << ")");
	}


	Window::~Window(){
		delete _config.input_manager;
		glfwDestroyWindow(_window);
		PH_LOG("Destroyed Window (" << _config.name << ")");
	}


	void Window::render(){

		// glfw swap buffers and poll I/O events
		glfwSwapBuffers(_window);
	}


	void Window::pollEvents(){
		glfwPollEvents();
	}


	///////////////////////////////////////////////////////////////////

	void Window::bind(){
		glfwMakeContextCurrent(_window);
	}


	// settings ///////////////////////////////////////////////////////


	void Window::setVSync(bool use){
		if(use){
			glfwSwapInterval(1);
			PH_INFO("(" << _config.name << ") VSync {enabled}");
		}else{
			glfwSwapInterval(0);
			PH_INFO("(" << _config.name << ") VSync {disabled}");
		};

		_config.VSync = use;
	}


	void Window::captureMouse(bool capture){
		if(capture){
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			PH_INFO("(" << _config.name << ") capture mouse {enabled}");
		}else{
			glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			PH_INFO("(" << _config.name << ") capture mouse {disabled}");
		};

		_config.captureMouse = capture;	
	}




	void Window::maximize(){
		glfwMaximizeWindow(_window);
	}

}
