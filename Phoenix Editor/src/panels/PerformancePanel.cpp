#include "ph_pch.h"
#include "PerformancePanel.h"

#include <Phoenix.h>
#include "../ImGui helpers.h"

namespace Phoenix{

	void PerformancePanel::render(Engine* editor){
		imgui_begin("1", "Performance");
			ImGui::Text("General:");
			ImGui::Indent();
				ImGui::Text("FPS:        %.1f", 1 / (editor->performanceMetrics.engineLoop / 1000000));
				ImGui::Text("Frame Time: %.3f ms", editor->performanceMetrics.engineLoop / 1000);
				ImGui::Text("Render:     %.3f ms", editor->performanceMetrics.renderLoop / 1000);
				ImGui::Text("Draw:       %.3f ms", editor->performanceMetrics.draw / 1000);
			ImGui::Unindent();

			imgui_separator();

			ImGui::Text("3D Rendering:");
			ImGui::Indent();
				ImGui::Text("Render:     %.3f ms", editor->performanceMetrics.render3D / 1000);
				ImGui::Text("Draw:       %.3f ms", editor->performanceMetrics.draw3D / 1000);
				ImGui::Text("Draw Calls: %d", editor->performanceMetrics.drawCalls3D);
				ImGui::Text("Verticies:  %d", editor->performanceMetrics.verticies3D);
				ImGui::Text("Indicies:   %d", editor->performanceMetrics.indicies3D);
			ImGui::Unindent();

			imgui_separator();

			ImGui::Text("2D Rendering:");
			ImGui::Indent();
				ImGui::Text("Render:     %.3f ms", editor->performanceMetrics.render2D / 1000);
				ImGui::Text("Draw:       %.3f ms", editor->performanceMetrics.draw2D / 1000);
				ImGui::Text("Draw Calls: %d", editor->performanceMetrics.drawCalls2D);
				ImGui::Text("Verticies:  %d", editor->performanceMetrics.verticies2D);
				ImGui::Text("Indicies:   %d", editor->performanceMetrics.indicies2D);
			ImGui::Unindent();

			imgui_separator();

			ImGui::Text("ECS:");
			ImGui::Indent();
				ImGui::Text("Entites:    %d", editor->performanceMetrics.entites);
				ImGui::Text("Update:     %.3f ms", editor->performanceMetrics.updateECS / 1000);
				ImGui::Text("Render:     %.3f ms", editor->performanceMetrics.renderECS / 1000);
			ImGui::Unindent();

		ImGui::End();
	}
	
}
