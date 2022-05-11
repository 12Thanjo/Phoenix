#include "SceneHierarchyPanel.h"

#include <Phoenix.h>
namespace Phoenix{


	void SceneHierarchyPanel::render(Engine* editor){
		ImGui::Begin("Scene Hierarchy");

			editor->_get_environment()._get_registry().each([&](auto entt_entity){
				Entity entity {entt_entity, &editor->_get_environment()};
				SceneHierarchyPanel::draw_entity_node(editor, entity);
			});




			if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
				selection_context = {};
			}



			if(ImGui::BeginPopupContextWindow(0, 1, false)){
				if(ImGui::MenuItem("Create Empty Entity")){
					editor->_get_environment().create_entity("Empty Entity");
				}

				ImGui::EndPopup();
			}

			ImGui::End();


			ImGui::Begin("Properties");
			if(selection_context){
				draw_components(selection_context);
			}

		ImGui::End();
	}




	void SceneHierarchyPanel::draw_entity_node(Engine* editor, Entity& entity){
		std::string& name = entity.getComponent<Component::Name>().name;

		// ImGui::Text(name.c_str());
		ImGuiTreeNodeFlags flags = ((selection_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if(ImGui::IsItemClicked()){
			selection_context = entity;
		}


		bool entity_deleted = false;
		if(ImGui::BeginPopupContextItem()){
			if(ImGui::MenuItem("Delete Entity")){
				opened = false;
				entity_deleted = true;
			}

			ImGui::EndPopup();
		}

		if(opened){
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)3251685, flags, name.c_str());
			if(opened){
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}


		if(entity_deleted){
			if(selection_context == entity){
				selection_context = {};
			}

			editor->_get_environment().destroy_entity(entity);
		}
	}



	static void draw_vec3_control(const std::string& label, glm::vec3& values, float reset_value = 0.0f, float collumn_width = 80.0f, float iteration_size = 1.0f){

		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[1];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);

		ImGui::SetColumnWidth(0, collumn_width);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

		float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 button_size = {line_height + 3.0f, line_height};



		// x input
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{RED_700});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{RED_600});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{RED_800});
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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{GREEN_700});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{GREEN_600});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{GREEN_800});
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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{BLUE_700});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{BLUE_600});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{BLUE_800});
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





	template<typename T, typename UIFuncton>
	static void draw_comonent(const std::string& name, Entity entity, UIFuncton ui_function){
		const ImGuiTreeNodeFlags tree_node_flags = 
			ImGuiTreeNodeFlags_DefaultOpen |
			ImGuiTreeNodeFlags_Framed |
			ImGuiTreeNodeFlags_SpanAvailWidth |
			ImGuiTreeNodeFlags_FramePadding |
			ImGuiTreeNodeFlags_AllowItemOverlap;
		

		if(entity.hasComponent<T>()){
			// ImGui::Separator();
			auto& component = entity.getComponent<T>();
			ImVec2 content_region_avail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
				float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::Separator();
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

			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		}
	}





	void SceneHierarchyPanel::draw_components(Entity& entity){
		if(entity.hasComponent<Component::Name>()){
			auto& name = entity.getComponent<Component::Name>().name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());
			ImGui::Text("Name:");
			if(ImGui::InputText("##", buffer, sizeof(buffer))){
				name = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

			if(ImGui::Button("Add Component")){
				ImGui::OpenPopup("AddComponent");
			}

			if(ImGui::BeginPopup("AddComponent")){
				int num_components = 2;

				if(!selection_context.hasComponent<Component::Transform>() && ImGui::MenuItem("Transform")){
					selection_context.addComponent<Component::Transform>();
					ImGui::CloseCurrentPopup();
				}else if(!selection_context.hasComponent<Component::SpriteRenderer>() && ImGui::MenuItem("Sprite Renderer")){
					selection_context.addComponent<Component::SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		ImGui::PopItemWidth();

		draw_comonent<Component::Transform>("Transform", entity, [](auto& component){
			draw_vec3_control("Position", component.translation);

			ImGui::Dummy(ImVec2(0.0f, 0.0f));

			glm::vec3 rotation = glm::degrees(component.rotation);
			draw_vec3_control("Rotation", rotation, 0.0f, 80.0f, 0.5f);
			component.rotation = glm::radians(rotation);

			ImGui::Dummy(ImVec2(0.0f, 0.0f));

			draw_vec3_control("Scale", component.scale, 100.0f);
		});


		draw_comonent<Component::SpriteRenderer>("Sprite Renderer", entity, [](auto& component){
			ImGui::ColorEdit4("Color", glm::value_ptr(component.color));
		});

	}
}
