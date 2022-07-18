#pragma once


#include <Phoenix.h>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"



namespace Phoenix{


	//////////////////////////////////////////////////////////////////////
	// internal state settings

	void imgui_set_collumn_width(float width);
	void imgui_set_collumn_width_default();


	//////////////////////////////////////////////////////////////////////
	// general
	
	void imgui_begin(std::string id, std::string name);

	void imgui_separator(float top_spacer = 5.0f);

	void imgui_spacer();
	void imgui_spacer(float y);
	void imgui_spacer(float x, float y);

	void imgui_columns(int collumns);

	void imgui_labled_item(const std::string& label, std::function<void()> draw_func);

	//////////////////////////////////////////////////////////////////////
	// controls

	void imgui_draw_float_control(const std::string& label, float& value, float iteration_size = 1.0f);
	void imgui_draw_vec3_control(const std::string& label, glm::vec3& values, float iteration_size = 1.0f, float reset_value = 0.0f);

	void imgui_draw_color_picker(const std::string & label, glm::vec4& color);
	void imgui_draw_color_picker(const std::string & label, glm::vec3& color);

	//////////////////////////////////////////////////////////////////////
	// buttons

	bool imgui_button(std::string label);
	bool imgui_button(std::string label, float width);
	bool imgui_button(std::string label, float width, float height);

	bool imgui_image_button(glID id, float width, float height, int padding = -1);
	bool imgui_image_button(Texture* texture, float width, float height, int padding = -1);


	//////////////////////////////////////////////////////////////////////
	// menus

	void imgui_draw_collapsable_menu(const std::string& label, std::function<void()> ui_function);

	void imgui_begin_disable_menu_item(bool disable, bool color = true);
	void imgui_end_disable_menu_item();


	void imgui_dropdown(const std::string& label, int& item, const char* items[], int num_items);


	//////////////////////////////////////////////////////////////////////
	// popups

	void imgui_start_alert(std::string text); // use this to start
	void imgui_start_alert(std::string title, std::string text); // use this to start
	void imgui_alert(); //call this automatically
	bool imugi_will_draw_alert();

	void imgui_popup(std::string title, std::function<void()> draw_window);

	void imgui_hover(std::string message);


	//////////////////////////////////////////////////////////////////////
	// helpers

	float imgui_get_line_height();
	float imgui_get_line_width();


}
