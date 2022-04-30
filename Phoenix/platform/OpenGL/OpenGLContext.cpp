#include "ph_pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Phoenix{

	OpenGLContext::OpenGLContext(GLFWwindow* p_window_handle)
		: window_handle(p_window_handle){
		ph_internal_assert(p_window_handle, "Window Handle is null");
	}


	void OpenGLContext::init(){
		glfwMakeContextCurrent(window_handle);
		if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
			ph_internal_fatal("Glad Initialization Failed");
		}

		ph_internal_info("OpenGL: \n\tVendor:   " << glGetString(GL_VENDOR) << "\n\tRenderer: " << glGetString(GL_RENDERER) << "\n\tDriver:   " << glGetString(GL_VERSION));
	}

	void OpenGLContext::swapBuffers(){
		glfwSwapBuffers(window_handle);
	}
}

