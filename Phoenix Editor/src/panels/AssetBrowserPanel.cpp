#include "ph_pch.h"
#include "AssetBrowserPanel.h"


#include <Phoenix.h>
#include "../ImGui helpers.h"
#include "../Application.h"

namespace Phoenix{

	// change when projects are used
	static std::filesystem::path project_path = "assets";
	// static std::filesystem::path project_path;



	AssetBrowserPanel::AssetBrowserPanel()
		: _current_dir(project_path) {

		_folder_icon = new Texture("assets/icons/file explorer/folder.png");
		_gray_file_icon = new Texture("assets/icons/file explorer/gray file.png");
	}


	AssetBrowserPanel::~AssetBrowserPanel(){
		delete _folder_icon;
		delete _gray_file_icon;
	}


	void AssetBrowserPanel::render(Engine* editor){
		imgui_begin("4", "Asset Browser");


		if(!project_path.empty()){
			if(_current_dir != project_path){
				if(ImGui::Button("<-")){
					_current_dir = _current_dir.parent_path();
				}
			}else{
				ImGui::Button("-");
			}

			ImGui::SameLine();
			ImGui::Text(_current_dir.string().c_str());

			imgui_separator();

			ImGui::BeginChild("Scroll");

				static float padding = 7.0f;
				static float thumbnail_size = 100.0f;
				static float cell_size = thumbnail_size + (padding*2);


				int column_number = (int)(ImGui::GetContentRegionAvail().x / cell_size);
				if(column_number > 0){
					imgui_columns(column_number);

						for(auto& dir : std::filesystem::directory_iterator(_current_dir)){

							const auto& dir_path = dir.path();
							auto relative_path = std::filesystem::relative(dir_path, project_path);
							std::string filename = relative_path.filename().string();

							ImGui::PushID(dir_path.string().c_str());

								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0,0,0,0});
									if(dir.is_directory()){
										imgui_image_button(_folder_icon->getID(), thumbnail_size, thumbnail_size);
										if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)){
											_current_dir /= dir_path.filename();
										}
									}else{
										imgui_image_button(_gray_file_icon->getID(), thumbnail_size, thumbnail_size);
									}

									if(ImGui::BeginDragDropSource()){
										const wchar_t* item_path = dir_path.c_str();
										ImGui::SetDragDropPayload("Asset Browser Item", item_path, (wcslen(item_path) + 1) * sizeof(wchar_t));


										ImGui::EndDragDropSource();
									}

								ImGui::PopStyleColor();

								ImGui::TextWrapped(filename.c_str());
								ImGui::NextColumn();
							ImGui::PopID();
						}

					imgui_columns(1);
				}

			ImGui::EndChild();
		}else{
			ImGui::Text("Open Project...");
			imgui_popup("Getting Started", [&](){
				ImGui::Text("Foundry Engine");

				imgui_separator(0.0f);

				// new project
				if(imgui_button("New", 40)){
					std::string new_project_path = static_cast<Editor*>(editor)->renderer_ImGui.newProject();
					if(!new_project_path.empty()){


						project_path = "assets";
						_current_dir = project_path;
					}
				}

				// open project
				if(imgui_button("Open", 40)){
					std::string opened_project_path = static_cast<Editor*>(editor)->renderer_ImGui.open();
					if(!opened_project_path.empty()){
						project_path = "assets";
						_current_dir = project_path;
					}
				}

			});
		}


		ImGui::End();
	}
	
}
