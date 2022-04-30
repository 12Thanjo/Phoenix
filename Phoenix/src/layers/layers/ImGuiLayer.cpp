#include "ph_pch.h"
#include "ImGuiLayer.h"

#include "Application.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>

namespace Phoenix{
	
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui") {

	}

	ImGuiLayer::~ImGuiLayer(){

	}


	void ImGuiLayer::onAttatch(){
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;



		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");


		ph_internal_info("ImGui Initialized");
	}

	void ImGuiLayer::onDetatch(){
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}



	void ImGuiLayer::begin(){
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::end(){
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::get();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}


	void ImGuiLayer::onImGuiRender(){
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
		ImGui::Text("%.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	}



}
