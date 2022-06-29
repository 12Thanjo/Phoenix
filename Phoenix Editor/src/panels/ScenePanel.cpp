#include "ph_pch.h"
#include "ScenePanel.h"

#include <Phoenix.h>
#include "../ImGui helpers.h"
#include "../Application.h"

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

				bool is_startup_scene = false;
				if(project.hasStartupScene()){
					is_startup_scene = project.getStartupScene() == scene->uuid;
				}

				bool checkbox_val = is_startup_scene;
				ImGui::Checkbox("Startup Scene", &checkbox_val);

				if(!is_startup_scene && checkbox_val){
					project.setStartupScene(scene->uuid);
				}

			});

			imgui_draw_collapsable_menu("Editor Camera", [&](){

				glm::vec3 focal_point = scene->camera.getFocalPoint();
				imgui_draw_vec3_control("Focal Point", focal_point, 0.0f, 70.0f, 0.5f);
				scene->camera.setFocalPoint(focal_point);

				imgui_separator(); ////////////////////////////////////////////////////////////////


				float rho = scene->camera.getRho();
				imgui_draw_float_control("Rho", rho, 10.0f, 40.0f, 0.5f);

				imgui_spacer();

				float theta = glm::degrees(scene->camera.getTheta());
				imgui_draw_float_control("Theta", theta, 1.5f, 40.0f, 0.5f);

				imgui_spacer();

				float phi = glm::degrees(scene->camera.getPhi());
				imgui_draw_float_control("Phi", phi, 1.5f, 40.0f, 0.5f);

				scene->camera.setCoordinates(rho, glm::radians(theta), glm::radians(phi));


				imgui_separator(); ////////////////////////////////////////////////////////////////


				float fov = glm::degrees(scene->camera.getFOV());
				imgui_draw_float_control("FOV", fov, 65.0f, 40.0f, 0.5f);

				imgui_spacer();

				float near = scene->camera.getNear();
				imgui_draw_float_control("Near", near, 0.1f, 40.0f, 0.1f);

				imgui_spacer();

				float far = scene->camera.getFar();
				imgui_draw_float_control("Far", far, 100.0f, 40.0f, 1.0f);


				scene->camera.setProjection(glm::radians(fov), scene->camera.getAspectRatio(), near, far);

			});

		ImGui::End();

	}
	
}
