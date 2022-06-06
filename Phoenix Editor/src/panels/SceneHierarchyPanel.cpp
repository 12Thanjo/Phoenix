#include "SceneHierarchyPanel.h"

#include <Phoenix.h>
#include "../ImGui helpers.h"


namespace Phoenix{
	
	void SceneHierarchyPanel::render(Engine* editor){
		// Scene Hierarchy Panel
		imgui_begin("2", "Scene Hierarchy");
			editor->getEnvironment()->forEach([&](Entity entity){
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
		imgui_begin("3", "Properties");
			if(_selection_context){
				draw_components(editor, _selection_context);
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

		// if(opened){
		// 	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		// 	bool opened = ImGui::TreeNodeEx((void*)3251685, flags, name.c_str());
		// 	if(opened){
		// 		ImGui::TreePop();
		// 	}
		// 	ImGui::TreePop();
		// }


		if(entity_deleted){
			if(_selection_context == entity){
				_selection_context = {};
			}

			editor->getEnvironment()->destroyEntity(entity);
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

		if(ImGui::BeginPopup("AddComponent")){
			int num_components = 2;

			if(!_selection_context.hasComponent<Component::Transform>() && ImGui::MenuItem("Transform")){
				_selection_context.addComponent<Component::Transform>();
				ImGui::CloseCurrentPopup();
			}else if(!_selection_context.hasComponent<Component::SpriteRenderer>() && ImGui::MenuItem("Sprite Renderer")){
				_selection_context.addComponent<Component::SpriteRenderer>();
				ImGui::CloseCurrentPopup();
			}else if(!_selection_context.hasComponent<Component::Camera>() && ImGui::MenuItem("Camera")){
				_selection_context.addComponent<Component::Camera>(glm::radians(65.0f), 16/9.0f, 0.1f, 100.0f);
				ImGui::CloseCurrentPopup();
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


		imgui_spacer();
		imgui_line();
		imgui_spacer();


		draw_comonent<Component::Transform>("Transform", entity, [](auto& component){
			imgui_draw_vec3_control("Position", component.translation, 0.0f, 60.0f, 0.01f);

			imgui_spacer();

			glm::vec3 rotation = glm::degrees(component.rotation);
			imgui_draw_vec3_control("Rotation", rotation, 0.0f, 60.0f, 0.5f);
			component.rotation = glm::radians(rotation);

			imgui_spacer();

			imgui_draw_vec3_control("Scale", component.scale, 0.5f, 60.0f, 0.01f);
		});


		draw_comonent<Component::SpriteRenderer>("Sprite Renderer", entity, [](auto& component){
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});

		draw_comonent<Component::Camera>("Camera", entity, [&entity, editor](auto& component){
			float fov = glm::degrees(component.camera.getFOV());
			imgui_draw_float_control("FOV", fov, 65.0f, 40.0f, 0.5f);

			imgui_spacer();

			float near = component.camera.getNear();
			imgui_draw_float_control("Near", near, 0.1f, 40.0f, 0.1f);

			imgui_spacer();

			float far = component.camera.getFar();
			imgui_draw_float_control("Far", far, 100.0f, 40.0f, 1.0f);




			component.camera.setProjection(glm::radians(fov), component.camera.getAspectRatio(), near, far);

			imgui_spacer(5.0f);
			imgui_line();
			imgui_spacer();


			bool primary = component.primary;
			ImGui::Checkbox("Primary Camera", &primary);

			if(!component.primary && primary){
				editor->setCamera(entity);
			}

		});

	}

}
