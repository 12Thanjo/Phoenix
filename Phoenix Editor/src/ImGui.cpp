#include <Phoenix.h>
#include "ImGui.h"

#include "ImGui helpers.h"

// panels
#include "panels/SceneHierarchyPanel.h"

namespace Phoenix{
	
	RendererImGui::RendererImGui(Engine* engine)
		: _editor(engine) {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata/static/Inconsolata/Inconsolata-Medium.ttf", 12.0f);
		io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata/static/Inconsolata/Inconsolata-ExtraBold.ttf", 12.0f);

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		set_dark_theme();
	}


	void RendererImGui::init(winID id){
		ImGui_ImplGlfw_InitForOpenGL(_editor->getWindow(id)->getWindowContext(), true);
		ImGui_ImplOpenGL3_Init("#version 410");


		_panels.push_back(static_cast<Panel*>(new SceneHierarchyPanel()));


		PH_INFO("ImGui Initialized");
	}


	RendererImGui::~RendererImGui(){
		for(auto panel : _panels){
			delete panel;
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}




	void RendererImGui::begin(){
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RendererImGui::end(){
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)_editor->windowWidth(0), (float)_editor->windowHeight(0));
		// io.DisplaySize = ImVec2(1280, 720);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}


	///////////////////////////////////////////////////////////////////////////////


	// void imgui_begin(std::string id, std::string name){
	// 	std::string imgui_id = name + "###" + id;
	// 	ImGui::Begin(imgui_id.c_str());
	// }

	///////////////////////////////////////////////////////////////////////////////

	void RendererImGui::render(FrameBuffer* render_buffer){
		static bool dockspaceOpen = true;
		if(dockspaceOpen){
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		    const ImGuiViewport* viewport = ImGui::GetMainViewport();
		    ImGui::SetNextWindowPos(viewport->WorkPos);
		    ImGui::SetNextWindowSize(viewport->WorkSize);
		    ImGui::SetNextWindowViewport(viewport->ID);
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode){
			    window_flags |= ImGuiWindowFlags_NoBackground;
			}

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

			ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);

		    ImGui::PopStyleVar();

		    ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			style.WindowMinSize.x = 200.0f;
			if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}


			if(ImGui::BeginMenuBar()){
			    if(ImGui::BeginMenu("File")){
			        if(ImGui::MenuItem("Exit")){ _editor->exit(); }
			        ImGui::MenuItem("Item 1");
			        ImGui::MenuItem("Item 2");

			        ImGui::EndMenu();
			    }


			    ImGui::EndMenuBar();
			}



			// viewport
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
				imgui_begin("viewport", "Viewport");
					// check if viewport resized
					ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
					if(content_region_avail.x != _viewport_size.x || content_region_avail.y != _viewport_size.y){
						_viewport_size = {content_region_avail.x, content_region_avail.y};
						render_buffer->resize(content_region_avail.x, content_region_avail.y);
						
						_editor->getEnvironment()->each<Component::Camera>([&](Entity entity, Component::Camera component){
							Camera& camera = component.camera;
							camera.setProjection(camera.getFOV(), content_region_avail.x/content_region_avail.y, camera.getNear(), camera.getFar());
						});
					}

					// draw from frame buffer
					uint32_t texture_id = render_buffer->getColorAttachment(0);
					ImGui::Image((void*)texture_id, ImVec2{content_region_avail.x, content_region_avail.y}, ImVec2{0,1}, ImVec2{1,0});
				ImGui::End();
			ImGui::PopStyleVar();



			// performance metrics
			imgui_begin("1", "Performance");
				ImGui::Text("General:");
			ImGui::End();


			// panels
			for(auto panel : _panels){
				panel->render(_editor);
			}


			ImGui::End();
		}



	}





	void RendererImGui::set_dark_theme(){
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg]           = ImVec4{ PH_GRAY_900 };

		// Headers
		colors[ImGuiCol_Header]        		= ImVec4{ PH_CYAN_800 };
		colors[ImGuiCol_HeaderHovered] 		= ImVec4{ PH_CYAN_700 };
		colors[ImGuiCol_HeaderActive]  		= ImVec4{ PH_CYAN_900 };

		// Buttons
		colors[ImGuiCol_Button]             = ImVec4{ PH_CYAN_800 };
		colors[ImGuiCol_ButtonHovered]      = ImVec4{ PH_CYAN_700 };
		colors[ImGuiCol_ButtonActive]       = ImVec4{ PH_CYAN_900 };

		// Frame Bg
		colors[ImGuiCol_FrameBg]            = ImVec4{ PH_GRAY_800 };
		colors[ImGuiCol_FrameBgHovered]     = ImVec4{ PH_GRAY_700 };
		colors[ImGuiCol_FrameBgActive]      = ImVec4{ PH_GRAY_600 };

		// Tabs
		colors[ImGuiCol_Tab] 				= ImVec4{ PH_CYAN_900 };
		colors[ImGuiCol_TabHovered] 		= ImVec4{ PH_CYAN_600 };
		colors[ImGuiCol_TabActive] 			= ImVec4{ PH_CYAN_700 };
		colors[ImGuiCol_TabUnfocused]       = ImVec4{ PH_CYAN_900 };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ PH_CYAN_900 };

		// Title Bg
		colors[ImGuiCol_TitleBg]            = ImVec4{ PH_GRAY_800 };
		colors[ImGuiCol_TitleBgActive]      = ImVec4{ PH_GRAY_700 };
		colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{ PH_GRAY_600 };
	}

}
