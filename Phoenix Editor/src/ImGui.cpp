#include <Phoenix.h>
#include "ImGui.h"

#include "ImGui helpers.h"

// panels
#include "panels/SceneHierarchyPanel.h"
#include "panels/AssetBrowserPanel.h"


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
		_panels.push_back(static_cast<Panel*>(new AssetBrowserPanel()));


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

	void RendererImGui::render(FrameBuffer* render_buffer, const winID& win_id){
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
			    	if(ImGui::MenuItem("New", "Ctrl+N")){
			    		PH_FATAL("This is an unsupported feature");
			    		PH_TRACE();
			    	}else if(ImGui::MenuItem("Open", "Ctrl+O")){
			    		open(win_id);
			    	}else if(ImGui::MenuItem("Save", "Ctrl+S")){
			    		save();
			    	}else if(ImGui::MenuItem("Save As", "Ctrl+Shift+S")){
			    		save_as(win_id);
			    	}
			    	imgui_separator();

			    	if(ImGui::MenuItem("Exit")){
			        	_editor->exit();
			        }

			        ImGui::EndMenu();
			    }


			    ImGui::EndMenuBar();
			}



			// viewport
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
				imgui_begin("viewport", "Viewport");
					// check if viewport resized
					ImVec2 content_region_avail = ImGui::GetContentRegionAvail();
					if(content_region_avail.x != _viewport_size.x || content_region_avail.y != _viewport_size.y || _just_opened){
						_viewport_size = {content_region_avail.x, content_region_avail.y};
						render_buffer->resize(content_region_avail.x, content_region_avail.y);
						
						_editor->getEnvironment()->each<Component::Camera>([&](Entity entity, Component::Camera& component){
							Camera& camera = component.camera;
							camera.setProjection(camera.getFOV(), content_region_avail.x/content_region_avail.y, camera.getNear(), camera.getFar());
						});


						_just_opened = false;
					}

					// draw from frame buffer
					uint32_t texture_id = render_buffer->getColorAttachment(0);
					ImGui::Image((void*)texture_id, ImVec2{content_region_avail.x, content_region_avail.y}, ImVec2{0,1}, ImVec2{1,0});

					if(ImGui::BeginDragDropTarget()){
						if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Asset Browser Item")){
							const wchar_t* path = (const wchar_t*)payload->Data;
							

							std::filesystem::path filesystem_path(path);
							if(filesystem_path.extension() == ".phoenix"){
								std::string path_string = filesystem_path.string();
								open(path_string);
							}else{
								PH_WARNING("Attempted to drag & load a non-scene");
							}

						}

						ImGui::EndDragDropTarget();
					}

				ImGui::End();
			ImGui::PopStyleVar();



			// performance metrics
			imgui_begin("1", "Performance");
				ImGui::Text("General:");
				ImGui::Indent();
					ImGui::Text("FPS:        %.1f", 1 / (_editor->performanceMetrics.engineLoop / 1000000));
					ImGui::Text("Frame Time: %.3f ms", _editor->performanceMetrics.engineLoop / 1000);
					ImGui::Text("Render:     %.3f ms", _editor->performanceMetrics.windows[win_id].renderLoop / 1000);
					ImGui::Text("Draw:       %.3f ms", _editor->performanceMetrics.windows[win_id].draw / 1000);
				ImGui::Unindent();

				imgui_separator();

				ImGui::Text("3D Rendering:");
				ImGui::Indent();
					ImGui::Text("Render:     %.3f ms", _editor->performanceMetrics.windows[win_id].render3D / 1000);
					ImGui::Text("Draw:       %.3f ms", _editor->performanceMetrics.windows[win_id].draw3D / 1000);
					ImGui::Text("Draw Calls: %d", _editor->performanceMetrics.windows[win_id].drawCalls3D);
					ImGui::Text("Verticies:  %d", _editor->performanceMetrics.windows[win_id].verticies3D);
					ImGui::Text("Indicies:   %d", _editor->performanceMetrics.windows[win_id].indicies3D);
				ImGui::Unindent();

				imgui_separator();

				ImGui::Text("2D Rendering:");
				ImGui::Indent();
					ImGui::Text("Render:     %.3f ms", _editor->performanceMetrics.windows[win_id].render2D / 1000);
					ImGui::Text("Draw:       %.3f ms", _editor->performanceMetrics.windows[win_id].draw2D / 1000);
					ImGui::Text("Draw Calls: %d", _editor->performanceMetrics.windows[win_id].drawCalls2D);
					ImGui::Text("Verticies:  %d", _editor->performanceMetrics.windows[win_id].verticies2D);
					ImGui::Text("Indicies:   %d", _editor->performanceMetrics.windows[win_id].indicies2D);
				ImGui::Unindent();

				imgui_separator();

				ImGui::Text("ECS:");
				ImGui::Indent();
					ImGui::Text("Entites:    %d", _editor->performanceMetrics.entites);
					ImGui::Text("Update:     %.3f ms", _editor->performanceMetrics.windows[win_id].updateECS / 1000);
					ImGui::Text("Render:     %.3f ms", _editor->performanceMetrics.windows[win_id].renderECS / 1000);
				ImGui::Unindent();

			ImGui::End();


			// panels
			for(auto panel : _panels){
				panel->render(_editor);
			}

			// ImGui::ShowDemoWindow();


			ImGui::End();
		}



	}





	void RendererImGui::set_dark_theme(){
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg]           = ImVec4{ PH_TRUE_GRAY_900 }; /*PH_GRAY_900*/

		// Headers
		colors[ImGuiCol_Header]        		= ImVec4{ PH_TRUE_GRAY_800 }; /*PH_CYAN_800*/
		colors[ImGuiCol_HeaderHovered] 		= ImVec4{ PH_TRUE_GRAY_700 }; /*PH_CYAN_700*/
		colors[ImGuiCol_HeaderActive]  		= ImVec4{ PH_TRUE_GRAY_800 }; /*PH_CYAN_900*/

		// Buttons
		colors[ImGuiCol_Button]             = ImVec4{ PH_TRUE_GRAY_800 }; /*PH_CYAN_800*/
		colors[ImGuiCol_ButtonHovered]      = ImVec4{ PH_TRUE_GRAY_700 }; /*PH_CYAN_700*/
		colors[ImGuiCol_ButtonActive]       = ImVec4{ PH_TRUE_GRAY_800 }; /*PH_CYAN_900*/

		// Frame Bg
		colors[ImGuiCol_FrameBg]            = ImVec4{ PH_TRUE_GRAY_800 }; /*PH_GRAY_800*/
		colors[ImGuiCol_FrameBgHovered]     = ImVec4{ PH_TRUE_GRAY_700 }; /*PH_GRAY_700*/
		colors[ImGuiCol_FrameBgActive]      = ImVec4{ PH_TRUE_GRAY_800 }; /*PH_GRAY_600*/

		// Tabs
		colors[ImGuiCol_Tab] 				= ImVec4{ PH_SKY_900 }; /*PH_CYAN_900*/
		colors[ImGuiCol_TabHovered] 		= ImVec4{ PH_SKY_600 }; /*PH_CYAN_600*/
		colors[ImGuiCol_TabActive] 			= ImVec4{ PH_SKY_700 }; /*PH_CYAN_700*/
		colors[ImGuiCol_TabUnfocused]       = ImVec4{ PH_SKY_900 }; /*PH_CYAN_900*/
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ PH_SKY_700 }; /*PH_CYAN_900*/

		// Title Bg
		colors[ImGuiCol_TitleBg]            = ImVec4{ PH_TRUE_GRAY_800 }; /*PH_GRAY_800*/
		colors[ImGuiCol_TitleBgActive]      = ImVec4{ PH_TRUE_GRAY_700 }; /*PH_GRAY_700*/
		colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{ PH_TRUE_GRAY_800 }; /*PH_GRAY_600*/
	}



	void RendererImGui::newScene(){
		_editor->clearEnvironment();
		_open_file = std::string();
	}

	void RendererImGui::open(winID win_id){
		std::string filepath = FileDialogs::open(*_editor->getWindow(win_id), "Phoenix Scene (*.phoenix)\0*.phoenix\0");

		if(!filepath.empty()){
			open(filepath);
		}
	}

	void RendererImGui::open(std::string filepath){
		_editor->clearEnvironment();

		_editor->deserialize(filepath);
		_open_file = filepath;

		// set primary camera
		_editor->getEnvironment()->each<Component::Camera>([&](Entity entity, auto& component){
			if(component.primary){
				_editor->setCamera(entity);

				glm::vec3& translation = entity.getComponent<Component::Transform>().translation;

			}
		});

		_just_opened = true;

		PH_LOG("Opened Scene: " << _open_file);
	}


	void RendererImGui::save(){
		if(!_open_file.empty()){
			_editor->serialize(_open_file);


			PH_LOG("Saved Scene: " << _open_file);
		}else{
			PH_LOG("Attempted to save non open file");
		}
	}

	void RendererImGui::save_as(winID win_id){
		std::string filepath = FileDialogs::save(*_editor->getWindow(win_id), "Phoenix Scene (*.phoenix)\0*.phoenix\0");

		if(!filepath.empty()){
			_open_file = filepath;
			save();
		}
	}

}
