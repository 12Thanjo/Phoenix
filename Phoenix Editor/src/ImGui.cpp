#include <Phoenix.h>
#include "ImGui.h"


#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"

#include "panels/SceneHierarchyPanel.h"

namespace Phoenix{
	
	RendererImGui::RendererImGui(Engine* engine)
		: editor(engine) {

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


		// GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(editor->getNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 410");


		Entity square = editor->createEntity("Square");

		square.addComponent<Component::Transform>(glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f)));
		square.addComponent<Component::SpriteRenderer>(glm::vec4{AMBER_700});

		class Test : public ScriptableEntity{
			private:
		
			public:
				void create(){
					// PH_LOG("Native Script Create");
				}

				void update(){
					// PH_LOG("Native Script Update");
				}
		};

		square.addComponent<Component::NativeScript>().bind<Test>();

		panels.push_back(static_cast<Panel*>(new SceneHierarchyPanel()));


		PH_INFO("ImGui Initialized");
	}

	RendererImGui::~RendererImGui(){
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		for(auto panel : panels){
			delete panel;
		}
	}



	void RendererImGui::begin(){
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RendererImGui::end(){
		ImGuiIO& io = ImGui::GetIO();
		// io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());
		io.DisplaySize = ImVec2(editor->getWidth(), editor->getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}


	void RendererImGui::render(Engine* editor){
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
			style.WindowMinSize.x = 330.0f;
			if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
			    ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}


			if(ImGui::BeginMenuBar()){
			    if(ImGui::BeginMenu("File")){
			        if(ImGui::MenuItem("Exit")){ editor->close(); }
			        ImGui::MenuItem("Item 1");
			        ImGui::MenuItem("Item 2");

			        ImGui::EndMenu();
			    }


			    ImGui::EndMenuBar();

			}


			// performance metrics
			ImGui::Begin("Performance Metrics");
				ImGui::Text("General:");
				ImGui::Indent();
					ImGui::Text("FPS:         %.1f", 1 / (editor->performanceMetrics.renderLoop / 1000000));
					ImGui::Text("Render:      %.3f ms", editor->performanceMetrics.renderLoop / 1000);
					ImGui::Text("Draw:        %.3f ms", editor->performanceMetrics.draw / 1000);
					ImGui::Text("ECS Systems: %.3f ms", editor->performanceMetrics.ECS / 1000);
					ImGui::Text("Update:      %.3f ms", editor->performanceMetrics.update / 1000);
				ImGui::Unindent();

				ImGui::Separator();

				ImGui::Text("Rendering:");
				ImGui::Indent();
					ImGui::Text("Draw Calls:  %d", editor->performanceMetrics.drawCalls);
					ImGui::Text("Verticies:   %d", editor->performanceMetrics.verticies);
					ImGui::Text("Indicies:    %d", editor->performanceMetrics.indicies);
				ImGui::Unindent();

				ImGui::Separator();

				ImGui::Text("2D Rendering:");
				ImGui::Indent();
					ImGui::Text("Draw Calls:  %d", editor->performanceMetrics.drawCalls2D);
					ImGui::Text("Quads:       %d", editor->performanceMetrics.quads);
					ImGui::Text("Verticies:   %d",  editor->performanceMetrics.verticies2D);
					ImGui::Text("Indicies:    %d",  editor->performanceMetrics.indicies2D);
				ImGui::Unindent();
				
				ImGui::Separator();

				ImGui::Text("ECS Environment:");
				ImGui::Indent();
					ImGui::Text("Entites:     %d", editor->performanceMetrics.entities);
				ImGui::Unindent();
			ImGui::End();


			for(auto panel : panels){
				panel->render(editor);
			}




			// viewport
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
				ImGui::Begin("Viewport");
					ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
					if(content_region_avail.x != viewport_size.x || content_region_avail.y != viewport_size.y){
						viewport_size = {content_region_avail.x, content_region_avail.y};
						editor->resize(content_region_avail.x, content_region_avail.y);
					}

					uint32_t texture_id = editor->getFrameBufferColorAttachment();
					ImGui::Image((void*)texture_id, ImVec2{content_region_avail.x, content_region_avail.y}, ImVec2{0,1}, ImVec2{1,0});
				ImGui::End();
			ImGui::PopStyleVar();




			ImGui::End();
		}
	}



	void RendererImGui::set_dark_theme(){
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg]           = ImVec4{ 0.094f, 0.094f, 0.105f, 1.0f };

		// Headers
		colors[ImGuiCol_Header]        		= ImVec4{ CYAN_800 };
		colors[ImGuiCol_HeaderHovered] 		= ImVec4{ CYAN_700 };
		colors[ImGuiCol_HeaderActive]  		= ImVec4{ CYAN_900 };

		// Buttons
		colors[ImGuiCol_Button]             = ImVec4{ CYAN_800 };
		colors[ImGuiCol_ButtonHovered]      = ImVec4{ CYAN_700 };
		colors[ImGuiCol_ButtonActive]       = ImVec4{ CYAN_900 };

		// Frame Bg
		colors[ImGuiCol_FrameBg]            = ImVec4{ BLUE_GRAY_800 };
		colors[ImGuiCol_FrameBgHovered]     = ImVec4{ BLUE_GRAY_700 };
		colors[ImGuiCol_FrameBgActive]      = ImVec4{ BLUE_GRAY_600 };

		// Tabs
		colors[ImGuiCol_Tab] 				= ImVec4{ CYAN_900 };
		colors[ImGuiCol_TabHovered] 		= ImVec4{ CYAN_600 };
		colors[ImGuiCol_TabActive] 			= ImVec4{ CYAN_700 };
		colors[ImGuiCol_TabUnfocused]       = ImVec4{ CYAN_900 };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ CYAN_900 };

		// Title Bg
		colors[ImGuiCol_TitleBg]            = ImVec4{ GRAY_800 };
		colors[ImGuiCol_TitleBgActive]      = ImVec4{ GRAY_700 };
		colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{ GRAY_600 };
	}

}

