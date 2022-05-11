#include "ph_pch.h"
#include <glad/glad.h>

#include "window.h"

#include "src/events/events.h"
#include "src/InputManager.h"




namespace Phoenix{

	static bool GLFW_initialized = false;

	static void glfw_error_callback(int error, const char* description){
		PH_ERROR("GLFW Error: (" << error << ")\n\t" << description);
	};
	
	Window::Window(const WindowProperties& props){
		data.name = props.name;
		data.width = props.width;
		data.height = props.height;
		data.use_frame_buffer = props.use_frame_buffer;


		// initialize glfw
		if(!GLFW_initialized){
			if(!glfwInit()){
				PH_FATAL("GLFW Initialization Failed");
			}
			glfwSetErrorCallback(glfw_error_callback);

			GLFW_initialized = true;
		}

		// create window
		window_context = glfwCreateWindow(data.width, data.height, data.name.c_str(), nullptr, nullptr);
		// window = glfwCreateWindow((int)props.width, (int)props.height, data.title.c_str(), glfwGetPrimaryMonitor(), nullptr);

		if(!window_context){
			PH_FATAL("Creating Window Failed");
		    glfwTerminate();
		}else{
			PH_INFO("Created Window (" << data.name << ") " << data.width << "x" << data.height);
		}



		// initialize glad
		glfwMakeContextCurrent(window_context);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			PH_FATAL("Glad Initialization Failed");
		}

		PH_INFO("OpenGL: \n\tVendor:   " << glGetString(GL_VENDOR)   << 
								 "\n\tRenderer: " << glGetString(GL_RENDERER) << 
								 "\n\tDriver:   " << glGetString(GL_VERSION)  <<
								 "\n\tShader:   " << glGetString(GL_SHADING_LANGUAGE_VERSION));

		
		glfwMakeContextCurrent(window_context);

		set_vsync(false);


		if(data.use_frame_buffer){
			FrameBufferConfig config;
			config.width = data.width;
			config.height = data.height;
			frame_buffer = CreateRef<FrameBuffer>(config);
		}


		// create window events //////////////////////////////////////////////////////////////////////////////////////////
		glfwSetWindowUserPointer(window_context, &data);

		// close window
		glfwSetWindowCloseCallback(window_context, [](GLFWwindow* window_context){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window_context);

			WindowCloseEvent e;

			data.event_callback(e);
		});

		// resize window
		glfwSetWindowSizeCallback(window_context, [](GLFWwindow* window_context, int width, int height){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window_context);

			data.width = width;
			data.height = height;

			WindowResizeEvent e(width, height);
			data.event_callback(e);
		});

		// keyboard
		glfwSetKeyCallback(window_context, [](GLFWwindow* window_context, int key, int scancode, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window_context);

			unsigned int ph_key = InputManager::glfwKeycodeToPhoenix(key);

			if(action == GLFW_PRESS){
				KeyDownEvent e(ph_key);
				data.event_callback(e);
			}else if(action == GLFW_RELEASE){
				KeyUpEvent e(ph_key);
				data.event_callback(e);
			}

		});


		// mouse buttons
		glfwSetMouseButtonCallback(window_context, [](GLFWwindow* window_context, int button, int action, int mods){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window_context);


			if(action == GLFW_PRESS ){
				MouseDownEvent e(button);
				data.event_callback(e);
			}else if(action == GLFW_RELEASE){
				MouseUpEvent e(button);
				data.event_callback(e);
			}
		});



		// mouse move
		glfwSetCursorPosCallback(window_context, [](GLFWwindow* window_context, double x, double y){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window_context);

			MouseMoveEvent e((float)x, (float)y);
			data.event_callback(e);
		});



		// mouse scroll
		glfwSetScrollCallback(window_context, [](GLFWwindow* window_context, double x, double y){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window_context);

			MouseScrollEvent e((float)x, (float)y);
			data.event_callback(e);
		});

	}

	Window::~Window(){
		glfwTerminate();
	}



	void Window::set_vsync(bool vsync){
		if(vsync){
			glfwSwapInterval(1);
			PH_INFO("VSync [Enabled]");
		}else{
			glfwSwapInterval(0);
			PH_INFO("VSync [Disabled]");
		};

		data.vsync = vsync;
	}


	void Window::clear(){
		if(data.use_frame_buffer){
			frame_buffer->bind();
		}
		glClearColor(0.058f, 0.09f, 0.164f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}


	void Window::update(){
		glfwSwapBuffers(window_context);
		glfwPollEvents();

		if(data.use_frame_buffer){
			frame_buffer->unbind();
		}
	}

}
