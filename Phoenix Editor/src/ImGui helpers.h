#pragma once


#include <Phoenix.h>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"



namespace Phoenix{
	
	void imgui_begin(std::string id, std::string name);

	void imgui_separator();

	void imgui_spacer();
	void imgui_spacer(float y);
	void imgui_spacer(float x, float y);

	void imgui_draw_float_control(const std::string& label, float value, float reset_value = 0.0f, float collumn_width = 60.0f, float iteration_size = 1.0f);
	void imgui_draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value = 0.0f, float collumn_width = 60.0f, float iteration_size = 1.0f);

}
