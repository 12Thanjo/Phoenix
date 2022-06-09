#include "ImGui helpers.h"


namespace Phoenix{
	
	void imgui_begin(std::string id, std::string name){
		std::string imgui_id = name + "###" + id;
		ImGui::Begin(imgui_id.c_str());
	}


	void imgui_separator(){
		ImGui::Separator();
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








	void imgui_draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value, float collumn_width, float iteration_size){

		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

			ImGui::Columns(2);

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

			ImGui::Columns(1);

		ImGui::PopID();
	}

	void imgui_draw_float_control(const std::string& label, float value, float reset_value, float collumn_width, float iteration_size){
		ImGui::PushID(label.c_str());

			ImGui::Columns(2);

			ImGui::SetColumnWidth(0, collumn_width);
			ImGui::Text(label.c_str());
			ImGui::NextColumn();


			ImGui::PushMultiItemsWidths(1, ImGui::GetContentRegionAvail().x - 5.0f);
				ImGui::DragFloat("##value", &value, iteration_size, 0.0f, 0.0f, "%.2f");
			ImGui::PopItemWidth();
			ImGui::SameLine();


			ImGui::Columns(1);

		ImGui::PopID();
	}

}
