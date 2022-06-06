#pragma once

#include <Phoenix.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

namespace Phoenix{
	
	class Panel{
		private:
	
		public:
			Panel() = default;
			~Panel() = default;

			virtual void render(Engine* editor) = 0;
			
	};

}
