#pragma once


#include <Phoenix.h>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"



namespace Phoenix{
	
	void imgui_begin(std::string id, std::string name);

	void imgui_separator(float top_spacer = 5.0f);

	void imgui_spacer();
	void imgui_spacer(float y);
	void imgui_spacer(float x, float y);

	void imgui_columns(int collumns);

	//////////////////////////////////////////////////////////////////////
	// controls

	void imgui_draw_float_control(const std::string& label, float& value, float reset_value = 0.0f, float collumn_width = 60.0f, float iteration_size = 1.0f);
	void imgui_draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value = 0.0f, float collumn_width = 60.0f, float iteration_size = 1.0f);


	//////////////////////////////////////////////////////////////////////
	// buttons

	bool imgui_button(std::string label);
	bool imgui_button(std::string label, float width);
	bool imgui_button(std::string label, float width, float height);
	bool imgui_image_button(glID id, float width, float height);



	//////////////////////////////////////////////////////////////////////
	// menus

	void imgui_draw_collapsable_menu(const std::string& label, std::function<void()> ui_function);

	void imgui_begin_disable_menu_item(bool disable, bool color = true);
	void imgui_end_disable_menu_item();


	//////////////////////////////////////////////////////////////////////
	// popups

	void imgui_start_alert(std::string text);
	void imgui_alert();

	void imgui_popup(std::string title, std::function<void()> draw_window);

}
