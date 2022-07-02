#include "ImGui helpers.h"


namespace Phoenix{
	
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


	void imgui_draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value, float collumn_width, float iteration_size){

		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

			ImGui::Columns(2, 0, false);

			ImGui::SetColumnWidth(0, collumn_width);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
				ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

				float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImVec2 button_size = {line_height, line_height};



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
					ImGui::DragFloat("##X", &values.x, iteration_size, 0.0f, 0.0f, "%.2f");
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
					ImGui::DragFloat("##Y", &values.y, iteration_size, 0.0f, 0.0f, "%.2f");
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
					ImGui::DragFloat("##Z", &values.z, iteration_size, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::SameLine();
				ImGui::PopStyleColor(3);


			ImGui::PopStyleVar();

			imgui_columns(1);

		ImGui::PopID();
	}

	void imgui_draw_float_control(const std::string& label, float& value, float reset_value, float collumn_width, float iteration_size){
		ImGui::PushID(label.c_str());

			imgui_columns(2);

			ImGui::SetColumnWidth(0, collumn_width);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();


			ImGui::PushMultiItemsWidths(1, ImGui::GetContentRegionAvail().x - 5.0f);
				ImGui::DragFloat("##value", &value, iteration_size, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();


			imgui_columns(1);

		ImGui::PopID();
	}



	//////////////////////////////////////////////////////////////////////
	// buttons

	bool imgui_button(std::string label){
		return ImGui::Button(label.c_str(), ImVec2{
			ImGui::GetContentRegionAvail().x,
			GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f
		});
	}


	bool imgui_button(std::string label, float width){
		return ImGui::Button(label.c_str(), ImVec2{
			width,
			GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f
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
		}

		imgui_spacer(0.0f, 10.0f);
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



	//////////////////////////////////////////////////////////////////////
	// popups

	static bool open_alert = false;
	static std::string alert_text = "Alert Text...";

	void imgui_alert(){
		if(open_alert){
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			

			ImGui::OpenPopup("Alert");
			if(ImGui::BeginPopupModal("Alert", NULL)){
				ImGui::SetWindowSize({320, 180});
			    ImGui::TextWrapped(alert_text.c_str());

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
		// imgui_alert();
	}

	bool imugi_will_draw_alert(){
		return open_alert;
	}




	void imgui_popup(std::string title, std::function<void()> draw_window){
		if(!open_alert){
			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			
			ImGui::OpenPopup(title.c_str());
			if(ImGui::BeginPopupModal(title.c_str(), NULL)){
				// PH_LOG("POPUP");
			   	draw_window();
			    ImGui::EndPopup();
			}
		}
	}



	//////////////////////////////////////////////////////////////////////
	// helpers


	float imgui_get_line_height(){
		return GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	}

}