#pragma once

#include <Phoenix.h>
#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"


namespace Phoenix{

	class Panel{
		public:
			virtual void render(Engine* editor) = 0;


		// ImGui Wrappers
		protected:
			// void window(std::string name, Engine* editor, void (*func)(Engine* editor));

	};
}
