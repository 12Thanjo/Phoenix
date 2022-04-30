#pragma once


#include "renderer/RenderContext.h"

struct GLFWwindow;


namespace Phoenix{

	class OpenGLContext : public RenderContext{
		private:
			GLFWwindow* window_handle;
	
		public:
			OpenGLContext(GLFWwindow* p_window_handle);


			void init() override;
			void swapBuffers() override;
	};
	
}
