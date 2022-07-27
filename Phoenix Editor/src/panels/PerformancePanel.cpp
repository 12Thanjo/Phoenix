#include "ph_pch.h"
#include "PerformancePanel.h"

#include <Phoenix.h>
#include "../ImGui helpers.h"


#define LABEL(title, format, val) imgui_labled_item(title, [&](){ \
		ImGui::Text(format, val); \
	});

namespace Phoenix{

	void PerformancePanel::render(Engine* editor){
		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[1];

		if(visible){
			imgui_begin("1", "Performance", &visible);
				imgui_set_collumn_width(70);

				ImGui::PushFont(bold_font);
					ImGui::Text("General:");
				ImGui::PopFont();
				ImGui::Indent();
					LABEL("FPS:", 		 "%.1f",    1 / (editor->performanceMetrics.engineLoop / 1000000));
					LABEL("Frame Time:", "%.3f ms", editor->performanceMetrics.engineLoop / 1000);
					LABEL("Render:", 	 "%.3f ms", editor->performanceMetrics.renderLoop / 1000);
				ImGui::Unindent();

				imgui_separator();

				ImGui::PushFont(bold_font);
					ImGui::Text("3D Rendering:");
				ImGui::PopFont();
				ImGui::Indent();
					LABEL("Render:", 	 "%.3f ms", editor->performanceMetrics.render3D / 1000);
					LABEL("Draw Calls:", "%d", 		editor->performanceMetrics.drawCalls3D);
					LABEL("Verticies:",  "%d", 		editor->performanceMetrics.verticies3D);
					LABEL("Indicies:", 	 "%d", 		editor->performanceMetrics.indicies3D);
				ImGui::Unindent();

				imgui_separator();

				ImGui::PushFont(bold_font);
					ImGui::Text("2D Rendering:");
				ImGui::PopFont();
				ImGui::Indent();
					LABEL("Render:",	 "%.3f ms", editor->performanceMetrics.render2D / 1000);
					LABEL("Draw Calls:", "%d", 		editor->performanceMetrics.drawCalls2D);
					LABEL("Verticies:",  "%d", 		editor->performanceMetrics.verticies2D);
					LABEL("Indicies:",	 "%d", 		editor->performanceMetrics.indicies2D);
				ImGui::Unindent();

				imgui_separator();

				ImGui::PushFont(bold_font);
					ImGui::Text("ECS:");
				ImGui::PopFont();
				ImGui::Indent();
					LABEL("Update:", 	 "%.3f ms", editor->performanceMetrics.updateECS / 1000);
					LABEL("Scripts:", 	 "%.3f ms", editor->performanceMetrics.scripts / 1000);
					LABEL("Entites:", 	 "%d", 		editor->performanceMetrics.entites);
				ImGui::Unindent();

			ImGui::End();
		}
	}
	
}
