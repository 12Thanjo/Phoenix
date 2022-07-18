#include "ImGui helpers.h"


namespace Phoenix{

	//////////////////////////////////////////////////////////////////////
	// internal state settings

	static float collumn_width = 60.0f;
	void imgui_set_collumn_width(float width){
		collumn_width = width;
	}
	void imgui_set_collumn_width_default(){
		collumn_width = 60.0f;
	}



	//////////////////////////////////////////////////////////////////////
	// general
	
	void imgui_begin(std::string id, std::string name){
		std::string imgui_id = name + "###" + id;
		ImGui::Begin(imgui_id.c_str(), nullptr);
	}


	void imgui_separator(float top_spacer){
		ImGui::Dummy(ImVec2(0.0f, top_spacer));
		ImGui::Separator();
		ImGui::Dummy(ImVec2(0.0f, 0.0f));
	}


	void imgui_spacer(){
		ImGui::Dummy(ImVec2(0.0f, 0.0f));
	}

	void imgui_spacer(float y){
		ImGui::Dummy(ImVec2(0.0f, y));
	}

	void imgui_spacer(float x, float y){
		ImGui::Dummy(ImVec2(x, y));
	}




	void imgui_columns(int columns){
		ImGui::Columns(columns, 0, false);
	}




	void imgui_labled_item(const std::string& label, std::function<void()> draw_func){
		ImGui::PushID(label.c_str());

			imgui_columns(2);

			ImGui::SetColumnWidth(0, collumn_width);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();


			ImGui::PushMultiItemsWidths(1, imgui_get_line_width());
				draw_func();
			ImGui::PopItemWidth();


			imgui_columns(1);

		ImGui::PopID();
	}

	//////////////////////////////////////////////////////////////////////
	// controls


	void imgui_draw_vec3_control(const std::string& label, glm::vec3& values, float iteration_size, float reset_value){

		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

			ImGui::Columns(2, 0, false);

			ImGui::SetColumnWidth(0, collumn_width);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

				float line_height = imgui_get_line_height();
				ImVec2 button_size = {line_height, line_height};


				ImGui::PushMultiItemsWidths(3, imgui_get_line_width() - (line_height * 3) + 8.0f );

				// x input
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{PH_RED_700});
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{PH_RED_600});
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{PH_RED_800});
					ImGui::PushFont(bold_font);
						if(ImGui::Button("X", button_size)){
							values.x = reset_value;
						}
					ImGui::PopFont();

					ImGui::SameLine();
					ImGui::DragFloat("##X", &values.x, iteration_size, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
					ImGui::PopItemWidth();
					ImGui::SameLine();
				ImGui::PopStyleColor(3);




				// y input 
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{PH_GREEN_700});
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{PH_GREEN_600});
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{PH_GREEN_800});
					ImGui::PushFont(bold_font);
						if(ImGui::Button("Y", button_size)){
							values.y = reset_value;
						}
					ImGui::PopFont();

					ImGui::SameLine();
					ImGui::DragFloat("##Y", &values.y, iteration_size, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
					ImGui::PopItemWidth();
					ImGui::SameLine();
				ImGui::PopStyleColor(3);




				// z input
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{PH_BLUE_700});
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{PH_BLUE_600});
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{PH_BLUE_800});
					ImGui::PushFont(bold_font);
						if(ImGui::Button("Z", button_size)){
							values.z = reset_value;
						}
					ImGui::PopFont();

					ImGui::SameLine();
					ImGui::DragFloat("##Z", &values.z, iteration_size, 0.0f, 0.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
					ImGui::PopItemWidth();
					ImGui::SameLine();
				ImGui::PopStyleColor(3);


			ImGui::PopStyleVar();

			imgui_columns(1);

		ImGui::PopID();
	}

	void imgui_draw_float_control(const std::string& label, float& value, float iteration_size){

		imgui_labled_item(label, [&](){
			ImGui::DragFloat("##value", &value, iteration_size, 0.0f, FLT_MAX, "%.2f", ImGuiSliderFlags_NoRoundToFormat | ImGuiSliderFlags_AlwaysClamp);
		});

	}


	void imgui_draw_color_picker(const std::string& label, glm::vec4& color){
		imgui_labled_item(label, [&](){
			const ImVec4 button_color = {color.r, color.g, color.b, color.a};
			static ImVec4 original_color = button_color;
			if(ImGui::ColorButton("##Color", button_color, ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip, ImVec2{imgui_get_line_width(), imgui_get_line_height()} )){
				ImGui::OpenPopup("##picker");
				original_color = button_color;
			}
			if(ImGui::BeginPopup("##picker")){
				ImGui::ColorPicker4("##color", glm::value_ptr(color), 
					ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar |
					ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview |
					ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex
				);

				ImGui::SameLine();

				ImGui::BeginGroup(); // Lock X position
					ImGui::Text("Current:");
					ImGui::ColorButton("##current", button_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));

					ImGui::Text("Original:");
					if(ImGui::ColorButton("##original", original_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40))){
					    color.r = original_color.x;
					    color.g = original_color.y;
					    color.b = original_color.z;
					    color.a = original_color.w;
					}
				ImGui::EndGroup();


				ImGui::EndPopup();
			}
		});
	}


	// no alpha
	void imgui_draw_color_picker(const std::string& label, glm::vec3& color){
		imgui_labled_item(label, [&](){
			const ImVec4 button_color = {color.r, color.g, color.b, 1.0f};
			static ImVec4 original_button_color = button_color;
			if(ImGui::ColorButton("##Color", button_color,  ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoTooltip, ImVec2{imgui_get_line_width(), imgui_get_line_height()} )){
				ImGui::OpenPopup("##picker");
				original_button_color = button_color;
			}
			if(ImGui::BeginPopup("##picker")){
				ImGui::ColorPicker3("##color", glm::value_ptr(color), 
					ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview |
					ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_DisplayHex
				);

				ImGui::SameLine();

				ImGui::BeginGroup(); // Lock X position
					ImGui::Text("Current:");
					ImGui::ColorButton("##current", button_color, ImGuiColorEditFlags_NoPicker, ImVec2(60, 40));

					ImGui::Text("Original:");
					if(ImGui::ColorButton("##original", original_button_color, ImGuiColorEditFlags_NoPicker, ImVec2(60, 40))){
					    color.r = original_button_color.x;
					    color.g = original_button_color.y;
					    color.b = original_button_color.z;
					}
				ImGui::EndGroup();


				ImGui::EndPopup();
			}
		});
	}



	//////////////////////////////////////////////////////////////////////
	// buttons

	bool imgui_button(std::string label){
		return ImGui::Button(label.c_str(), ImVec2{
			imgui_get_line_width(),
			imgui_get_line_height()
		});
	}

	bool imgui_button(std::string label, float width){
		return ImGui::Button(label.c_str(), ImVec2{
			width,
			imgui_get_line_height()
		});
	}

	bool imgui_button(std::string label, float width, float height){
		return ImGui::Button(label.c_str(), ImVec2{width, height});
	}


	bool imgui_image_button(glID id, float width, float height, int padding){
		return ImGui::ImageButton((ImTextureID)id, {width, height}, {0, 1}, {1, 0}, padding);
	}

	bool imgui_image_button(Texture* texture, float width, float height, int padding){
		return ImGui::ImageButton((ImTextureID)texture->getID(), {width, height}, {0, 1}, {1, 0}, padding);
	}



	//////////////////////////////////////////////////////////////////////
	// menus

	void imgui_draw_collapsable_menu(const std::string& label, std::function<void()> ui_function){
		imgui_set_collumn_width_default();

		const ImGuiTreeNodeFlags tree_node_flags = 
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_FramePadding |
			ImGuiTreeNodeFlags_AllowItemOverlap;

		// ImGui::Separator();
		// auto& component = entity.getComponent<T>();
		// ImVec2 content_region_avail = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
			float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			bool open = ImGui::TreeNodeEx(label.c_str(), tree_node_flags, label.c_str());
		ImGui::PopStyleVar();

		if(open){
			ui_function();
			
			ImGui::TreePop();
			
			imgui_spacer(0.0f, 10.0f);
		}

	}



	void imgui_begin_disable_menu_item(bool disable, bool color){
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disable);
		if(disable && color){
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
		}else{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 1.0f);
		}
	}


	void imgui_end_disable_menu_item(){
		ImGui::PopItemFlag();
		ImGui::PopStyleVar();
	}



	void imgui_dropdown(const std::string& label, int& item, const char* items[], int num_items){

		imgui_labled_item(label, [&](){
			ImGui::Combo("##value", &item, items, num_items);
		});

	}



	//////////////////////////////////////////////////////////////////////
	// popups

	static bool open_alert = false;
	static std::string alert_text = "Alert Text...";
	static std::string alert_title = "Alert";

	void imgui_alert(){
		if(open_alert){
			// ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			// ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			

			ImGui::OpenPopup(alert_title.c_str());
			if(ImGui::BeginPopupModal(alert_title.c_str(), NULL)){
				ImGui::SetWindowSize({400, 225});

				ImGui::BeginChild("Scroll", {385, 168});
			    	ImGui::TextWrapped(alert_text.c_str());
			    ImGui::EndChild();

			    if(ImGui::Button("Close")){
			        ImGui::CloseCurrentPopup();
			        open_alert = false;
			    }
			    ImGui::EndPopup();
			}
		}
	}


	void imgui_start_alert(std::string text){
		alert_text = text;
		open_alert = true;
		alert_title = "Alert";
		PH_WARNING("Alert: " << text);
	}

	void imgui_start_alert(std::string title, std::string text){
		alert_text = text;
		open_alert = true;
		alert_title = title;
	}

	bool imugi_will_draw_alert(){
		return open_alert;
	}




	void imgui_popup(std::string title, std::function<void()> draw_window){
		if(!open_alert){
			// ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			// ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			
			ImGui::OpenPopup(title.c_str());
			if(ImGui::BeginPopupModal(title.c_str(), NULL)){
				// PH_LOG("POPUP");
			   	draw_window();
			    ImGui::EndPopup();
			}
		}
	}



	void imgui_hover(std::string message){
		if(ImGui::IsItemHovered()){
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(message.c_str());
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}



	//////////////////////////////////////////////////////////////////////
	// helpers

	float imgui_get_line_width(){
		return ImGui::GetContentRegionAvail().x - 5.0f;
	}

	float imgui_get_line_height(){
		return GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	}

}