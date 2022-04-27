#include "ph_pch.h"

#include "WindowsWindow.h"

#include "events/events.h"

namespace Phoenix{
	static bool GLFW_initialized = false;

	static void glfw_error_callback(int error, const char* description){
		ph_internal_error("GLFW Error: (" << error << ")\n\t" << description);
	};

	Window* Window::create(const WindowProperties& props){
		return new WindowsWindow(props);
	}


	WindowsWindow::WindowsWindow(const WindowProperties& props){
		init(props);
	}

	WindowsWindow::~WindowsWindow(){
		destroy();
	}


	void WindowsWindow::init(const WindowProperties& props){
		data.title = props.title;
		data.width = props.width;
		data.height = props.height;

		if(!GLFW_initialized){
			if(!glfwInit()){
				ph_internal_fatal("GLFW Initialization Failed");
			}

			glfwSetErrorCallback(glfw_error_callback);




			GLFW_initialized = true;
		}

		window = glfwCreateWindow((int)props.width, (int)props.height, data.title.c_str(), nullptr, nullptr);

		if(!window){
			ph_internal_fatal("Creating Window Failed");
		    glfwTerminate();
		}else{
			ph_internal_info("Created Windows Window (" << data.title << ") " << props.width << "x" << props.height);
		}

		// setVSync(false);

		glfwMakeContextCurrent(window);

		// create window events //////////////////////////////////////////////////////////////////////////////////////////
		glfwSetWindowUserPointer(window, &data);

		glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			data.width = width;
			data.height = height;


			ph_internal_info("Resized window: " << width << "x" << height);

			WindowResizeEvent e(width, height);
			data.event_callback(e);
		});

		// custom cursor
		{
			unsigned char pixels[16 * 16 * 4];
			memset(pixels, 0xff, sizeof(pixels));

			for(int i = 0; i < 16*16; i+=4){
				pixels[i] = 0xee;
				pixels[i + 1] = 0x11;
				pixels[i + 2] = 0x11;
			}

			for(int i = 16*16; i < 16*16*2; i+=4){
				pixels[i] = 0xff;
				pixels[i + 1] = 0xcc;
				pixels[i + 2] = 0x11;
			}

			for(int i = 16*16*2; i < 16*16*3; i+=4){
				pixels[i] = 0x00;
				pixels[i + 1] = 0xcc;
			}

			for(int i = 16*16*3; i < 16*16*4; i+=4){
				pixels[i] = 0xcc;
				pixels[i + 1] = 0x11;
				pixels[i + 2] = 0xff;
			}

			 
			GLFWimage image;
			image.width = 16;
			image.height = 16;
			image.pixels = pixels;
			 
			GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
			glfwSetCursor(window, cursor);
		}
	}

	void WindowsWindow::destroy(){
		glfwDestroyWindow(window);
		ph_internal_info("window (" << data.title<< ") shut down");
	}


	void WindowsWindow::set_vsync(bool enabled){
		if(enabled){
			glfwSwapInterval(1);
		}else{
			glfwSwapInterval(0);
		};

		ph_internal_info("window (" << data.title<< ") VSync set to: " << enabled);

		data.VSync = enabled;
	}

	void WindowsWindow::run(){
		glClearColor(0, 0.06f, 0.13f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

