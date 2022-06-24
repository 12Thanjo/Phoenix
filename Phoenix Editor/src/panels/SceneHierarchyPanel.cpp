#include "SceneHierarchyPanel.h"

#include <Phoenix.h>
#include "../ImGui helpers.h"




namespace Phoenix{
	
	void SceneHierarchyPanel::render(Engine* editor){
		// Scene Hierarchy Panel
		imgui_begin("2", "Scene Hierarchy");
			editor->getScene()->forEach([&](Entity entity){
				draw_entity_node(editor, entity);
			});


			if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
				_selection_context = {};
			}


			if(ImGui::BeginPopupContextWindow(0, 1, false)){
				if(ImGui::MenuItem("Create Empty Entity")){
					editor->createEntity("Empty Entity");
				}

				ImGui::EndPopup();
			}
		ImGui::End();



		// properties
		imgui_begin("3", "Entity Properties");
			if(_selection_context){
				draw_components(editor, _selection_context);
			}else{
				ImGui::Text("No Entity Selected");
			}
		ImGui::End();

	}


	void SceneHierarchyPanel::draw_entity_node(Engine* editor, Entity& entity){
		std::string& name = entity.getComponent<Component::Name>().name;

		// ImGui::Text(name.c_str());
		ImGuiTreeNodeFlags flags = ((_selection_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if(ImGui::IsItemClicked()){
			_selection_context = entity;
		}


		bool entity_deleted = false;
		if(ImGui::BeginPopupContextItem()){
			if(ImGui::MenuItem("Delete Entity")){
				opened = false;
				entity_deleted = true;
			}

			ImGui::EndPopup();
		}



		if(entity_deleted){
			if(_selection_context == entity){
				_selection_context = {};
			}

			editor->destroyEntity(entity);
		}
	}




	template<typename T, typename UIFuncton>
	static void draw_comonent(const std::string& name, Entity entity, UIFuncton ui_function){
		
		if(entity.hasComponent<T>()){

			const ImGuiTreeNodeFlags tree_node_flags = 
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_AllowItemOverlap;

			// ImGui::Separator();
			auto& component = entity.getComponent<T>();
			ImVec2 content_region_avail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
				float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				bool open = ImGui::TreeNodeEx(name.c_str(), tree_node_flags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(content_region_avail.x - (line_height * 0.5f) + 2);
			if(ImGui::Button("=", ImVec2{line_height, line_height})){
				ImGui::OpenPopup("ComponentSettings");
			}

			bool remove_component = false;
			if(ImGui::BeginPopup("ComponentSettings")){
				if(ImGui::MenuItem("Remove Component")){
					remove_component = true;
				}

				ImGui::EndPopup();
			}

			if(open){
				// auto& color = entity.getComponent<T>().color;
				// ImGui::ColorEdit4("Color", glm::value_ptr(color));

				ui_function(component);
				
				ImGui::TreePop();
			}


			if(remove_component){
				entity.removeComponent<T>();
			}

			imgui_spacer(0.0f, 10.0f);
		}
	}

	template<typename T>
	static void draw_empty_comonent(const std::string& name, Entity entity){
		
		if(entity.hasComponent<T>()){

			const ImGuiTreeNodeFlags tree_node_flags = 
				ImGuiTreeNodeFlags_DefaultOpen |
				ImGuiTreeNodeFlags_Framed |
				ImGuiTreeNodeFlags_SpanAvailWidth |
				ImGuiTreeNodeFlags_FramePadding |
				ImGuiTreeNodeFlags_AllowItemOverlap;

			// ImGui::Separator();
			// auto& component = entity.getComponent<T>();
			ImVec2 content_region_avail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
				float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				bool open = ImGui::TreeNodeEx(name.c_str(), tree_node_flags, name.c_str());
			ImGui::PopStyleVar();

			ImGui::SameLine(content_region_avail.x - (line_height * 0.5f) + 2);
			if(ImGui::Button("=", ImVec2{line_height, line_height})){
				ImGui::OpenPopup("ComponentSettings");
			}

			bool remove_component = false;
			if(ImGui::BeginPopup("ComponentSettings")){
				if(ImGui::MenuItem("Remove Component")){
					remove_component = true;
				}

				ImGui::EndPopup();
			}

			if(open){
				// auto& color = entity.getComponent<T>().color;
				// ImGui::ColorEdit4("Color", glm::value_ptr(color));

				// ui_function(component);
				
				ImGui::TreePop();
			}


			if(remove_component){
				entity.removeComponent<T>();
			}

			imgui_spacer(0.0f, 10.0f);
		}
	}




	void SceneHierarchyPanel::draw_components(Engine* editor, Entity& entity){
		auto& name = entity.getComponent<Component::Name>().name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), name.c_str());
		ImGui::Text("Name:");

		ImGui::SameLine();
		if(ImGui::InputText("##", buffer, sizeof(buffer))){
			name = std::string(buffer);
		}


		if(ImGui::Button("Add Component")){
			ImGui::OpenPopup("AddComponent");
		}

		// if(){
		// 	if(ImGui::MenuItem("Create Empty Entity")){
		// 		editor->createEntity("Empty Entity");
		// 	}

		// 	ImGui::EndPopup();
		// }

		if(ImGui::BeginPopup("AddComponent") || ImGui::BeginPopupContextWindow(0, 1, false)){
			bool has_transform = _selection_context.hasComponent<Component::Transform>();
			bool has_script = _selection_context.hasComponent<Component::Script>();

			bool has_perspective = _selection_context.hasComponent<Component::PerspectiveCamera>();
			bool has_orbital = _selection_context.hasComponent<Component::OrbitalCamera>();
			bool has_camera = has_perspective || has_orbital;

			bool has_sprite = _selection_context.hasComponent<Component::SpriteRenderer>();
			bool has_cube = _selection_context.hasComponent<Component::Cube>();
			bool has_renderer = has_sprite || has_cube;

			///////////////////////////////////////////////

			ImGuiIO& io = ImGui::GetIO();
			auto bold_font = io.Fonts->Fonts[1];

			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::PushFont(bold_font);
				ImGui::MenuItem("Add Component:");
			ImGui::PopFont();
			ImGui::PopItemFlag();
			imgui_separator(0.0f);

			///////////////////////////////////////////////

			imgui_begin_disable_menu_item(has_transform, true);
				if(ImGui::MenuItem("Transform")){
					_selection_context.addComponent<Component::Transform>();
					ImGui::CloseCurrentPopup();
				}
			imgui_end_disable_menu_item();


			imgui_begin_disable_menu_item(has_script, true);
				if(ImGui::MenuItem("Script")){
					_selection_context.addComponent<Component::Script>();
					ImGui::CloseCurrentPopup();
				}
			imgui_end_disable_menu_item();

	
		    if(ImGui::BeginMenu("Cameras", !has_camera && !has_renderer)){
				if(ImGui::MenuItem("Perspective Camera")){
					_selection_context.addComponent<Component::PerspectiveCamera>(glm::radians(65.0f), 16/9.0f, 0.1f, 100.0f);
					if(!has_transform){
						_selection_context.addComponent<Component::Transform>();
					}
				}else if(ImGui::MenuItem("Orbital Camera", "", has_transform)){
					_selection_context.addComponent<Component::OrbitalCamera>(glm::radians(65.0f), 16/9.0f, 0.1f, 100.0f);
				}
		        ImGui::EndMenu();
		    }


		    imgui_separator(0.0f);


		    if(ImGui::BeginMenu("2D Render Objects", !has_renderer && !has_camera)){
				if(ImGui::MenuItem("Sprite Renderer")){
					_selection_context.addComponent<Component::SpriteRenderer>();
					if(!has_transform){
						_selection_context.addComponent<Component::Transform>(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
					}
				}

		        ImGui::EndMenu();
		    }


		    if(ImGui::BeginMenu("3D Render Objects", !has_renderer && !has_camera)){
				if(ImGui::MenuItem("Cube")){
					_selection_context.addEmptyComponent<Component::Cube>();
					if(!has_transform){
						_selection_context.addComponent<Component::Transform>();
					}
				}

		        ImGui::EndMenu();
		    }



			ImGui::EndPopup();
		}


		ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
		if(content_region_avail.x > 240){
			// ImGui::SameLine(content_region_avail.x - 170);
			ImGui::SameLine();
			std::string uuid = "UUID: " + std::to_string((uint64_t)entity.getComponent<Component::UUID>().id);
			ImGui::Text(uuid.c_str());

		}


		imgui_separator(0.0f);

		draw_comonent<Component::Transform>("Transform", entity, [&entity](auto& component){
			imgui_draw_vec3_control("Position", component.position, 0.0f, 60.0f, 0.01f);


			imgui_spacer();

			glm::vec3 rotation = glm::degrees(component.rotation);
			imgui_draw_vec3_control("Rotation", rotation, 0.0f, 60.0f, 0.5f);
			component.rotation = glm::radians(rotation);


			if(!entity.hasComponent<Component::PerspectiveCamera>()){
				imgui_spacer();
				imgui_draw_vec3_control("Scale", component.scale, 0.5f, 60.0f, 0.01f);
			}

		});


		draw_comonent<Component::Script>("Script", entity, [&entity](auto& component){
			float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			// ImGui::Columns(2, 0, false);
			ImGui::Text("Script:");
			ImGui::SameLine();
			std::string button_label;

			if(!component.path.empty()){
				button_label = component.path;
			}else{
				button_label = "Add Script...";
			};


			if(imgui_button(button_label, ImGui::GetContentRegionAvail().x, line_height) && !component.path.empty()){
				// Do nothing when clicked (at least for now)
			}

			if(ImGui::BeginDragDropTarget()){
				if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset Browser Item")){
					const wchar_t* path = (const wchar_t*)payload->Data;

					std::filesystem::path filesystem_path(path);
					if(filesystem_path.extension() == ".js"){
						std::string path_string = filesystem_path.string();
						component.path = path_string;
					}else{
						PH_WARNING("Attempted to drag & load a non-script");
					}
				}
				ImGui::EndDragDropTarget();
			}
		});


		draw_comonent<Component::SpriteRenderer>("Sprite Renderer", entity, [](auto& component){
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});

		draw_comonent<Component::Cube>("Cube", entity, [](auto& component){
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});


		//////////////////////////////////////////////////////////////////////
		// Cameras

		draw_comonent<Component::PerspectiveCamera>("Perspective Camera", entity, [&entity, editor](auto& component){
			float fov = glm::degrees(component.camera.getFOV());
			imgui_draw_float_control("FOV", fov, 65.0f, 40.0f, 0.5f);

			imgui_spacer();

			float near = component.camera.getNear();
			imgui_draw_float_control("Near", near, 0.1f, 40.0f, 0.1f);

			imgui_spacer();

			float far = component.camera.getFar();
			imgui_draw_float_control("Far", far, 100.0f, 40.0f, 1.0f);


			component.camera.setProjection(glm::radians(fov), component.camera.getAspectRatio(), near, far);


			imgui_separator(); ////////////////////////////////////////////////////////////////


			bool currently_viewing = editor->usingCamera(entity);
			bool start_viewing = editor->usingCamera(entity);
			ImGui::Checkbox("View Perspective", &start_viewing);

			if(!currently_viewing && start_viewing){
				editor->setCamera(entity);
			}else if(currently_viewing && !start_viewing){
				editor->setCamera({});
			}

		});



		draw_comonent<Component::OrbitalCamera>("Orbital Camera", entity, [&entity, editor](auto& component){

			glm::vec3 focal_point = component.camera.getFocalPoint();
			imgui_draw_vec3_control("Focal Point", focal_point, 0.0f, 70.0f, 0.5f);
			component.camera.setFocalPoint(focal_point);

			imgui_separator(); ////////////////////////////////////////////////////////////////


			float rho = component.camera.getRho();
			imgui_draw_float_control("Rho", rho, 10.0f, 40.0f, 0.5f);

			imgui_spacer();

			float theta = glm::degrees(component.camera.getTheta());
			imgui_draw_float_control("Theta", theta, 1.5f, 40.0f, 0.5f);

			imgui_spacer();

			float phi = glm::degrees(component.camera.getPhi());
			imgui_draw_float_control("Phi", phi, 1.5f, 40.0f, 0.5f);

			component.camera.setCoordinates(rho, glm::radians(theta), glm::radians(phi));


			imgui_separator(); ////////////////////////////////////////////////////////////////


			float fov = glm::degrees(component.camera.getFOV());
			imgui_draw_float_control("FOV", fov, 65.0f, 40.0f, 0.5f);

			imgui_spacer();

			float near = component.camera.getNear();
			imgui_draw_float_control("Near", near, 0.1f, 40.0f, 0.1f);

			imgui_spacer();

			float far = component.camera.getFar();
			imgui_draw_float_control("Far", far, 100.0f, 40.0f, 1.0f);


			component.camera.setProjection(glm::radians(fov), component.camera.getAspectRatio(), near, far);


			imgui_separator(); ////////////////////////////////////////////////////////////////
 


			bool currently_viewing = editor->usingCamera(entity);
			bool start_viewing = editor->usingCamera(entity);
			ImGui::Checkbox("View Perspective", &start_viewing);

			if(!currently_viewing && start_viewing){
				editor->setCamera(entity);
			}else if(currently_viewing && !start_viewing){
				editor->setCamera({});
			}

		});

	}

}
