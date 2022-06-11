#include "ph_pch.h"
#include "AssetBrowserPanel.h"


#include <Phoenix.h>
#include "../ImGui helpers.h"





namespace Phoenix{

	// chance when projects are used
	static const std::filesystem::path project_path = "assets";


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

			float padding = 5.0f;
			float thumbnail_size = 100.0f;
			float cell_size = thumbnail_size + padding;


			int collumn_number = (int)(ImGui::GetContentRegionAvail().x / cell_size);
			ImGui::Columns(collumn_number, 0, false);

				for(auto& dir : std::filesystem::directory_iterator(_current_dir)){
					const auto& dir_path = dir.path();
					auto relative_path = std::filesystem::relative(dir_path, project_path);
					std::string filename = relative_path.filename().string();



					if(dir.is_directory()){
						imgui_image_button(_folder_icon->getID(), thumbnail_size, thumbnail_size);
						// ImGui::Button("Directory", {thumbnail_size, thumbnail_size});
						if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)){
							_current_dir /= dir_path.filename();
						}
					}else{
						// ImGui::Button("File", {thumbnail_size, thumbnail_size});
						imgui_image_button(_gray_file_icon->getID(), thumbnail_size, thumbnail_size);
						// if(ImGui::Button(filename.c_str())){
							
						// }
					}

					ImGui::TextWrapped(filename.c_str());
					ImGui::NextColumn();
				}

			ImGui::Columns(1);

		ImGui::EndChild();

		ImGui::End();
	}
	
}
