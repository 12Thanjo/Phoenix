#include "ph_pch.h"
#include "ScenePanel.h"

#include <Phoenix.h>
#include "../ImGui helpers.h"
#include "../Application.h"
#include "SceneHierarchyPanel.h"

namespace Phoenix{

	void ScenePanel::render(Engine* editor){

		imgui_begin("scene", "Scene");
			auto* scene = editor->getScene();

			auto& name = scene->name;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), name.c_str());
			ImGui::Text("Name:");

			ImGui::SameLine();
			if(ImGui::InputText("##", buffer, sizeof(buffer))){
				name = std::string(buffer);
			}


			std::string uuid = "UUID: " + std::to_string(scene->uuid);
			ImGui::Text(uuid.c_str());

			imgui_separator();



			//////////////////////////////////////////////////////////////////////
			// Editor Camera

			imgui_draw_collapsable_menu("General", [&](){
				Project& project = static_cast<Editor*>(editor)->project;

				//////////////////////////////////////////////////////////////////////
				// starutp scene

				bool is_startup_scene = false;
				if(project.hasStartupScene()){
					is_startup_scene = project.getStartupScene() == scene->uuid;
				}

				bool checkbox_val = is_startup_scene;
				ImGui::Checkbox("Startup Scene", &checkbox_val);

				if(!is_startup_scene && checkbox_val){
					project.setStartupScene(scene->uuid);
				}

				//////////////////////////////////////////////////////////////////////
				// startup camera

				std::string startup_camera;
				if(scene->hasStartupCamera()){
					startup_camera = std::to_string(scene->getStartupCamera());
				}else{
					startup_camera = "None Set";
				}

				// startup_camera = "Startup Camera: " + startup_camera;
				// ImGui::Text(startup_camera.c_str());

				ImGui::Text("Startup Camera");
				ImGui::SameLine();
				if(imgui_button(startup_camera) && scene->hasStartupCamera()){
					// set selected entity to the startup camera
					static_cast<SceneHierarchyPanel*>(
						static_cast<Editor*>(editor)->renderer_ImGui.scene_hierarchy_panel
					)->selection_context = scene->getEntityByUUID( scene->getStartupCamera() );
				}

			});

			imgui_draw_collapsable_menu("Editor Camera", [&](){
				imgui_set_collumn_width(70.0f);

				glm::vec3 focal_point = scene->camera.getFocalPoint();
				imgui_draw_vec3_control("Focal Point", focal_point, 0.0f, 0.5f);
				scene->camera.setFocalPoint(focal_point);

				imgui_separator(); ////////////////////////////////////////////////////////////////

				imgui_set_collumn_width(40.0f);

				float rho = scene->camera.getRho();
				imgui_draw_float_control("Rho", rho, 0.5f);


				float theta = glm::degrees(scene->camera.getTheta());
				imgui_draw_float_control("Theta", theta, 0.5f);


				float phi = glm::degrees(scene->camera.getPhi());
				imgui_draw_float_control("Phi", phi, 0.5f);

				scene->camera.setCoordinates(rho, glm::radians(theta), glm::radians(phi));


				imgui_separator(); ////////////////////////////////////////////////////////////////


				float fov = glm::degrees(scene->camera.getFOV());
				imgui_draw_float_control("FOV", fov, 0.5f);


				float near = scene->camera.getNear();
				imgui_draw_float_control("Near", near, 0.1f);


				float far = scene->camera.getFar();
				imgui_draw_float_control("Far", far, 1.0f);


				scene->camera.setProjection(glm::radians(fov), scene->camera.getAspectRatio(), near, far);

			});


			imgui_draw_collapsable_menu("Sunlight", [&](){
				imgui_draw_color_picker("Color", scene->sunlight.color);

				imgui_draw_float_control("Strength", scene->sunlight.strength, 0.05f);
				
				imgui_separator();

				imgui_draw_vec3_control("Direction", scene->sunlight.direction, -1.0f, 0.02f);

			});

		ImGui::End();

	}
	
}
