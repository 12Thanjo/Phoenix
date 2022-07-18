#include "SceneHierarchyPanel.h"

#include <Phoenix.h>

#include "../ImGui helpers.h"
#include "../Application.h"
#include "../editor events.h"


namespace Phoenix{
	
	//////////////////////////////////////////////////////////////////////
	// panel

	void SceneHierarchyPanel::render(Engine* editor){
		// Scene Hierarchy Panel
		imgui_begin("2", "Scene Hierarchy");
			editor->getScene()->forEach([&](Entity entity){
				draw_entity_node(editor, entity);
			});


			if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()){
				selection_context = {};
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
			if(selection_context){
				draw_components(editor, selection_context);
			}else{
				ImGui::Text("No Entity Selected");
			}
		ImGui::End();
	}


	void SceneHierarchyPanel::onEvent(Event& e, Engine* editor){
		EventType type = e.getType();

		bool ctrl_down = editor->keyDown(PH_KEY_LEFT_CONTROL) || editor->keyDown(PH_KEY_RIGHT_CONTROL);
		bool shift_down = editor->keyDown(PH_KEY_LEFT_SHIFT) || editor->keyDown(PH_KEY_RIGHT_SHIFT);

		switch(type){
			case PH_MOUSE_DOWN_EVENT:
				if(static_cast<MouseDownEvent&>(e).getButton() == PH_MOUSE_LEFT){
					if(static_cast<Editor*>(editor)->renderer_ImGui.getMouseOverViewport()){
						if(current_state == SHP_State::None){
							selection_context = {(entt::entity)static_cast<Editor*>(editor)->renderer_ImGui.getEntityIdMouseOver(), editor->getScene()};
							if(!selection_context){
								current_state = SHP_State::None;
							}
						}else if(current_state == SHP_State::Grab){
							finish_tool();
						}else if(current_state == SHP_State::Rotate){
							finish_tool();
						}else if(current_state == SHP_State::Scale){
							finish_tool();
						}

					}
				}
				break;
			case PH_KEY_DOWN_EVENT:
				switch(static_cast<KeyDownEvent&>(e).getKeycode()){
					case PH_KEY_ESCAPE:
						reset_property();
						finish_tool();
						selection_context = {};
						break;

					//////////////////////////////////////////////////////////////////////
					// property delimiters
					case PH_KEY_X:
						if(current_state == SHP_State::Grab || current_state == SHP_State::Rotate || current_state == SHP_State::Scale){
							reset_property();
							if(!shift_down){
								_can_adjust_x = true;
								_can_adjust_y = false;
								_can_adjust_z = false;
							}else{
								_can_adjust_x = false;
								_can_adjust_y = true;
								_can_adjust_z = true;
							}
						}
						break;
					case PH_KEY_Y:
						if(current_state == SHP_State::Grab || current_state == SHP_State::Rotate || current_state == SHP_State::Scale){
							reset_property();
							if(!shift_down){
								_can_adjust_x = false;
								_can_adjust_y = true;
								_can_adjust_z = false;
							}else{
								_can_adjust_x = true;
								_can_adjust_y = false;
								_can_adjust_z = true;
							}
						}
						break;
					case PH_KEY_Z:
						if(current_state == SHP_State::Grab || current_state == SHP_State::Rotate || current_state == SHP_State::Scale){
							reset_property();
							if(!shift_down){
								_can_adjust_x = false;
								_can_adjust_y = false;
								_can_adjust_z = true;
							}else{
								_can_adjust_x = true;
								_can_adjust_y = true;
								_can_adjust_z = false;
							}
						}
						break;
				};
				break;
			case PH_MOUSE_MOVE_EVENT:
				{
					float current_mouse_x = editor->mouseX();
					float current_mouse_y = editor->mouseY();

					//////////////////////////////////////////////////////////////////////
					// grab
					if(current_state == SHP_State::Grab){
						float dx = current_mouse_x - _mouse_x;
						float dy = current_mouse_y - _mouse_y;

						OrbitalCamera& camera = editor->getScene()->camera;
						glm::vec3 camera_rotation = camera.getRotation();
						glm::vec3 camera_position = camera.getPosition();

						glm::vec3& position = selection_context.getComponent<Component::Transform>().position;

						glm::vec3 position_dif = abs(camera_position - position);


						float ammount = sqrt(
							(position_dif.x * position_dif.x) + 
							(position_dif.y * position_dif.y) + 
							(position_dif.z * position_dif.z)
						) * 0.0015f;


						position.z += dx * ammount * cos(camera_rotation.y) 							* _can_adjust_z;
						position.x -= dx * ammount * sin(camera_rotation.y) 							* _can_adjust_x;

						position.y -= dy * ammount * cos(camera_rotation.z) 							* _can_adjust_y;
						position.x += dy * ammount * sin(camera_rotation.z) * cos(camera_rotation.y) 	* _can_adjust_x;
						position.z += dy * ammount * sin(camera_rotation.z) * sin(camera_rotation.y) 	* _can_adjust_z;

					//////////////////////////////////////////////////////////////////////
					// rotate
					}else if(current_state == SHP_State::Rotate){
						float dx = current_mouse_x - _mouse_x;
						float dy = current_mouse_y - _mouse_y;

						OrbitalCamera& camera = editor->getScene()->camera;
						glm::vec3 camera_rotation = camera.getRotation();
						glm::vec3 camera_position = camera.getPosition();

						glm::vec3 position = selection_context.getComponent<Component::Transform>().position;
						glm::vec3& rotation = selection_context.getComponent<Component::Transform>().rotation;

						glm::vec3 position_dif = abs(camera_position - position);


						float ammount = sqrt(
							(position_dif.x * position_dif.x) + 
							(position_dif.y * position_dif.y) + 
							(position_dif.z * position_dif.z)
						) * 0.0015f;



						rotation.x -= dx * ammount * cos(camera_rotation.y) 	* _can_adjust_x;
						rotation.y += dx * ammount * cos(camera_rotation.z) 	* _can_adjust_y;


						rotation.z += dy * ammount * cos(camera_rotation.y) 	* _can_adjust_z;
						// rotation.y += dy * ammount * cos(camera_rotation.y) 	* _can_adjust_y;
						// rotation.x -= dy * ammount * sin(camera_rotation.y) 	* _can_adjust_x;

					}
					//////////////////////////////////////////////////////////////////////
					// scale
					if(current_state == SHP_State::Scale){
						float dx = current_mouse_x - _mouse_x;
						float dy = current_mouse_y - _mouse_y;

						OrbitalCamera& camera = editor->getScene()->camera;
						glm::vec3 camera_rotation = camera.getRotation();
						glm::vec3 camera_position = camera.getPosition();

						glm::vec3 position = selection_context.getComponent<Component::Transform>().position;
						glm::vec3& scale = selection_context.getComponent<Component::Transform>().scale;

						glm::vec3 position_dif = abs(camera_position - position);


						float ammount = sqrt(
							(position_dif.x * position_dif.x) + 
							(position_dif.y * position_dif.y) + 
							(position_dif.z * position_dif.z)
						) * 0.0015f;


						scale.z += dx * ammount * cos(camera_rotation.y) 							* _can_adjust_z;
						scale.x -= dx * ammount * sin(camera_rotation.y) 							* _can_adjust_x;

						scale.y -= dy * ammount * cos(camera_rotation.z) 							* _can_adjust_y;
						scale.x += dy * ammount * sin(camera_rotation.z) * cos(camera_rotation.y) 	* _can_adjust_x;
						scale.z += dy * ammount * sin(camera_rotation.z) * sin(camera_rotation.y) 	* _can_adjust_z;

					//////////////////////////////////////////////////////////////////////
					// rotate
					}


					_mouse_x = current_mouse_x;
					_mouse_y = current_mouse_y;
				}
				break;
			case GRAB_EVENT:
				if(selection_context){
					if(current_state == SHP_State::Grab){
						finish_tool();
					}else{
						reset_property();
						_original_vec3 = selection_context.getComponent<Component::Transform>().position;
						current_state = SHP_State::Grab;
					}
				}
				break;
			case ROTATE_EVENT:
				if(selection_context){
					if(current_state == SHP_State::Rotate){
						finish_tool();
					}else{
						reset_property();
						_original_vec3 = selection_context.getComponent<Component::Transform>().rotation;
						current_state = SHP_State::Rotate;
					}
				}
				break;
			case SCALE_EVENT:
				if(selection_context){
					if(current_state == SHP_State::Scale){
						finish_tool();
					}else{
						reset_property();
						_original_vec3 = selection_context.getComponent<Component::Transform>().scale;
						current_state = SHP_State::Scale;
					}
				}
				break;
		};
	}



	void SceneHierarchyPanel::reset_property(){
		if(current_state == SHP_State::Grab){
			selection_context.getComponent<Component::Transform>().position = _original_vec3;
		}else if(current_state == SHP_State::Rotate){
			selection_context.getComponent<Component::Transform>().rotation = _original_vec3;
		}else if(current_state == SHP_State::Scale){
			selection_context.getComponent<Component::Transform>().scale = _original_vec3;
		}
	}



	void SceneHierarchyPanel::finish_tool(){
		current_state = SHP_State::None;
		_can_adjust_x = true;
		_can_adjust_y = true;
		_can_adjust_z = true;

	}




	//////////////////////////////////////////////////////////////////////
	// helpers

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
			ImGui::TreePop();
		}



		if(entity_deleted){
			if(selection_context == entity){
				selection_context = {};
			}

			editor->destroyEntity(entity);
		}
	}





	template<typename T, typename UIFuncton>
	static void draw_comonent(const std::string& name, Entity entity, UIFuncton ui_function){
		
		if(entity.hasComponent<T>()){
			imgui_set_collumn_width_default();

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
				float line_height = imgui_get_line_height();
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
				ui_function(component);
				
				ImGui::TreePop();

				imgui_spacer(0.0f, 10.0f);
			}


			if(remove_component){
				entity.removeComponent<T>();
			}

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


	template<typename T>
	static void draw_material(T& component, Engine* editor){
		const char* list[] = { "None", "Image Texture" };
		int using_texture = component.using_texture;
		imgui_dropdown("Texture", using_texture, list, 2);

		//////////////////////////////////////////////////////////////////////
		// texture dropdown on-change
		if(using_texture == 1 && !component.using_texture){
			component.using_texture = true;

			if(component.has_save_texture){
				component.material.setTexture(component.save_texture);
			}

		}else if(using_texture == 0 && component.using_texture){
			component.using_texture = false;
			component.material.useColor();
		}

		//////////////////////////////////////////////////////////////////////
		// texture button
		if(using_texture == 1){
			// texture drag UI
			imgui_labled_item("Image", [&, editor](){
				std::string texture;

				if(component.material.usingTexture()){
					texture = Files::relative(
						static_cast<Editor*>(editor)->project.textures.getLeft(component.material.getTexture()),
						"\\textures\\"
					);
					// texture = static_cast<Editor*>(editor)->project.textures.getLeft(component.material.getTexture());
					// texture = std::to_string(component.material.getTexture());
				}else{
					texture = "[Drag Image...]";
				}


				if(imgui_button(texture)){/*do nothing right now*/}
				if(ImGui::BeginDragDropTarget()){
					if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset Browser Item")){
						const wchar_t* path = (const wchar_t*)payload->Data;

						std::filesystem::path filesystem_path(path);
						if(filesystem_path.extension() == ".png"){
							UUID texture_id = static_cast<Editor*>(editor)->project.textures.getRight(
								Files::relative(filesystem_path.string(), static_cast<Editor*>(editor)->project.getRelativePath())
							);

							component.material.setTexture(texture_id);

							component.save_texture = texture_id;
							component.has_save_texture = true;
						}else{
							PH_WARNING("Attempted to drag & load a non-texture");
						}
					}
					ImGui::EndDragDropTarget();
				}
			});
		}

		imgui_separator(0.0f);

		imgui_draw_color_picker("Color", component.material.color);

		float shininess = component.material.shininess;
		shininess = sqrt(shininess);
		imgui_draw_float_control("Shininess", shininess, 0.2f);

		if(shininess > 0){
			component.material.shininess = shininess * shininess;
		}else{
			component.material.shininess = 0;
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
			bool has_transform = selection_context.hasComponent<Component::Transform>();
			bool has_script = selection_context.hasComponent<Component::Script>();

			bool has_perspective = selection_context.hasComponent<Component::PerspectiveCamera>();
			bool has_orbital = selection_context.hasComponent<Component::OrbitalCamera>();
			bool has_camera = has_perspective || has_orbital;

			bool has_sprite = selection_context.hasComponent<Component::SpriteRenderer>();
			bool has_cube = selection_context.hasComponent<Component::Cube>();
			bool has_plane = selection_context.hasComponent<Component::Plane>();
			bool has_renderer = has_sprite || has_cube || has_plane;

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
					selection_context.addComponent<Component::Transform>();
					ImGui::CloseCurrentPopup();
				}
			imgui_end_disable_menu_item();


			imgui_begin_disable_menu_item(has_script, true);
				if(ImGui::MenuItem("Script")){
					selection_context.addComponent<Component::Script>();
					ImGui::CloseCurrentPopup();
				}
			imgui_end_disable_menu_item();

	
		    if(ImGui::BeginMenu("Cameras", !has_camera && !has_renderer)){
				if(ImGui::MenuItem("Perspective Camera")){
					selection_context.addComponent<Component::PerspectiveCamera>(glm::radians(65.0f), 16/9.0f, 0.1f, 100.0f);
					if(!has_transform){
						selection_context.addComponent<Component::Transform>();
					}
				}else if(ImGui::MenuItem("Orbital Camera", "", has_transform)){
					selection_context.addComponent<Component::OrbitalCamera>(glm::radians(65.0f), 16/9.0f, 0.1f, 100.0f);
				}
		        ImGui::EndMenu();
		    }


		    imgui_separator(0.0f);


		    if(ImGui::BeginMenu("2D Render Objects", !has_renderer && !has_camera)){
				if(ImGui::MenuItem("Sprite Renderer")){
					selection_context.addComponent<Component::SpriteRenderer>();
					if(!has_transform){
						selection_context.addComponent<Component::Transform>(glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
					}
				}

		        ImGui::EndMenu();
		    }


		    if(ImGui::BeginMenu("3D Render Objects", !has_renderer && !has_camera)){
				if(ImGui::MenuItem("Cube")){
					selection_context.addEmptyComponent<Component::Cube>();
					if(!has_transform){
						selection_context.addComponent<Component::Transform>();
					}
				}else if(ImGui::MenuItem("Plane")){
					selection_context.addEmptyComponent<Component::Plane>();
					if(!has_transform){
						selection_context.addComponent<Component::Transform>();
					}
				}

		        ImGui::EndMenu();
		    }



			ImGui::EndPopup();
		}


		ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
		if(content_region_avail.x > 245){
			// ImGui::SameLine(content_region_avail.x - 170);
			ImGui::SameLine();
			ImGui::Text("UUID:");
			ImGui::SameLine();

			std::string uuid = std::to_string((uint64_t)entity.getComponent<Component::UUID>().id);

			if(ImGui::Button(uuid.c_str())){
				PH_LOG("Copied to clipboard: " << uuid);
				Files::toClipboard(uuid);
			}

		}


		imgui_separator(0.0f);

		draw_comonent<Component::Transform>("Transform", entity, [&entity](auto& component){
			imgui_set_collumn_width_default();

			imgui_draw_vec3_control("Position", component.position, 0.01f, 0.0f);

			imgui_spacer();

			glm::vec3 rotation = glm::degrees(component.rotation);
			imgui_draw_vec3_control("Rotation", rotation, 0.5f, 0.0f);
			component.rotation = glm::radians(rotation);


			if(!entity.hasComponent<Component::PerspectiveCamera>()){
				imgui_spacer();
				imgui_draw_vec3_control("Scale", component.scale, 0.01f, 0.5f);
			}

		});


		draw_comonent<Component::Script>("Script", entity, [&entity](auto& component){
			float line_height = imgui_get_line_height();

			// ImGui::Columns(2, 0, false);
			ImGui::Text("Script:");
			ImGui::SameLine();
			std::string button_label;

			if(!component.path.empty()){
				button_label = Files::getFileName(component.path);
			}else{
				button_label = "[Drag Script...]";
			};


			if(imgui_button(button_label, imgui_get_line_width() - 4.0f) && !component.path.empty()){
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

		draw_comonent<Component::Cube>("Cube", entity, [&](auto& component){
			draw_material<Component::Cube>(component, editor);
		});


		draw_comonent<Component::Plane>("Plane", entity, [&](auto& component){
			draw_material<Component::Plane>(component, editor);
		});


		//////////////////////////////////////////////////////////////////////
		// Cameras

		draw_comonent<Component::PerspectiveCamera>("Perspective Camera", entity, [&entity, editor](auto& component){
			imgui_set_collumn_width(40.0f);

			float fov = glm::degrees(component.camera.getFOV());
			imgui_draw_float_control("FOV", fov, 0.5f);

			float near = component.camera.getNear();
			imgui_draw_float_control("Near", near, 0.1f);

			float far = component.camera.getFar();
			imgui_draw_float_control("Far", far, 1.0f);


			component.camera.setProjection(glm::radians(fov), component.camera.getAspectRatio(), near, far);


			imgui_separator(); ////////////////////////////////////////////////////////////////

			UUID& id = entity.getComponent<Component::UUID>().id;
			Scene* scene = editor->getScene();

			bool currently_startup_camera = scene->hasStartupCamera() && (scene->getStartupCamera() == id);
			bool set_startup_camera = currently_startup_camera;
			ImGui::Checkbox("Startup Camera", &set_startup_camera);

			if(!currently_startup_camera && set_startup_camera){
				scene->setStartupCamera(id);
			}


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
			imgui_set_collumn_width(70.0f);

			glm::vec3 focal_point = component.camera.getFocalPoint();
			imgui_draw_vec3_control("Focal Point", focal_point, 0.0f, 0.5f);
			component.camera.setFocalPoint(focal_point);

			imgui_separator(); ////////////////////////////////////////////////////////////////

			imgui_set_collumn_width(40.0f);

			float rho = component.camera.getRho();
			imgui_draw_float_control("Rho", rho, 0.5f);

			float theta = glm::degrees(component.camera.getTheta());
			imgui_draw_float_control("Theta", theta, 0.5f);

			float phi = glm::degrees(component.camera.getPhi());
			imgui_draw_float_control("Phi", phi, 0.5f);

			component.camera.setCoordinates(rho, glm::radians(theta), glm::radians(phi));


			imgui_separator(); ////////////////////////////////////////////////////////////////

			imgui_set_collumn_width(40.0f);

			float fov = glm::degrees(component.camera.getFOV());
			imgui_draw_float_control("FOV", fov, 0.5f);

			float near = component.camera.getNear();
			imgui_draw_float_control("Near", near, 0.1f);

			float far = component.camera.getFar();
			imgui_draw_float_control("Far", far, 1.0f);


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
