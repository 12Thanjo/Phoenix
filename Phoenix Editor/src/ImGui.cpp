#include <Phoenix.h>
#include "ImGui.h"

#include "Application.h"
#include "ImGui helpers.h"

// panels
#include "panels/SceneHierarchyPanel.h"
#include "panels/AssetBrowserPanel.h"
#include "panels/PerformancePanel.h"
#include "panels/ScenePanel.h"


namespace Phoenix{
	
	RendererImGui::RendererImGui(Engine* engine)
		: _editor(engine) {
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;


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


	void RendererImGui::init(){
		ImGui_ImplGlfw_InitForOpenGL(_editor->getWindow()->getWindowContext(), true);
		ImGui_ImplOpenGL3_Init("#version 410");


		_panels.push_back(static_cast<Panel*>(new ScenePanel()));
		_panels.push_back(static_cast<Panel*>(new SceneHierarchyPanel()));
		_panels.push_back(static_cast<Panel*>(new AssetBrowserPanel()));
		_panels.push_back(static_cast<Panel*>(new PerformancePanel()));


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
		io.DisplaySize = ImVec2((float)_editor->windowWidth(), (float)_editor->windowHeight());
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

	void RendererImGui::render(FrameBuffer* render_buffer){
		static bool dockspaceOpen = true;
		if(dockspaceOpen){
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			dockspace_flags |= ImGuiDockNodeFlags_NoWindowMenuButton; //get rid of window menu button

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
			    		open();
			    	}else if(ImGui::MenuItem("Save", "Ctrl+S")){
			    		save();
			    	}else if(ImGui::MenuItem("Save As", "Ctrl+Shift+S")){
			    		save_as();
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
						
						_editor->getScene()->each<Component::PerspectiveCamera>([&](Entity entity, Component::PerspectiveCamera& component){
							PerspectiveCamera& camera = component.camera;
							camera.setProjection(camera.getFOV(), content_region_avail.x/content_region_avail.y, camera.getNear(), camera.getFar());
						});

						_editor->getScene()->each<Component::OrbitalCamera>([&](Entity entity, Component::OrbitalCamera& component){
							OrbitalCamera& camera = component.camera;
							camera.setProjection(camera.getFOV(), content_region_avail.x/content_region_avail.y, camera.getNear(), camera.getFar());
						});


						OrbitalCamera& camera = _editor->getScene()->camera;
						camera.setProjection(camera.getFOV(), content_region_avail.x/content_region_avail.y, camera.getNear(), camera.getFar());

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
								open_scene(path_string);
							}else{
								PH_WARNING("Attempted to drag & load a non-scene");
							}

						}

						ImGui::EndDragDropTarget();
					}

				ImGui::End();
			ImGui::PopStyleVar();



			// panels
			for(auto panel : _panels){
				panel->render(_editor);
			}

			// ImGui::ShowDemoWindow();

			imgui_alert();


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
		_editor->clearScene();
		_open_file = std::string();
	}

	std::string RendererImGui::newProject(){
		return FileDialogs::save(*_editor->getWindow(), {
			.title = "Create Project",
			.filters = {
				{"Phoenix Project", ""}
			}
		});
	}

	std::string RendererImGui::open(){
		std::string filepath = FileDialogs::open(*_editor->getWindow(), {
			.filters = {
				{"Phoenix Scene (*.phoenix)", "*.phoenix"}
			}
		});

		if(!filepath.empty()){
			open(filepath);
		}

		return filepath;
	}

	// void RendererImGui::open(std::string filepath){
	// 	_editor->clearScene();

	// 	std::string deserialize = _editor->deserialize(filepath);
	// 	if(deserialize.empty()){
	// 		_open_file = filepath;
	// 		_just_opened = true;

	// 		PH_LOG("Opened Scene: " << _open_file);
	// 	}else{
	// 		imgui_start_alert(deserialize);
	// 		newScene();
	// 	}
	// }

	void RendererImGui::open(std::string filepath){
		PH_WARNING("Open doesn't do anything");
		PH_TRACE();
	}


	void RendererImGui::open_scene(std::string filepath){
		_editor->clearScene();

		std::string deserialize = _editor->deserialize(filepath);
		if(deserialize.empty()){
			_open_file = filepath;
			_just_opened = true;

			PH_LOG("Opened Scene: " << _open_file);
		}else{
			imgui_start_alert(deserialize);
			newScene();
		}
	}


	void RendererImGui::save(){
		if(!_open_file.empty()){
			_editor->serialize(_open_file);


			PH_LOG("Saved Scene: " << _open_file);
		}else{
			PH_LOG("Attempted to save non open file");
		}
	}

	void RendererImGui::save_as(){
		std::string filepath = FileDialogs::save(*_editor->getWindow(), {
			.filters = {
				{"Phoenix Scene (*.phoenix)", "*.phoenix"}
			}
		});

		if(!filepath.empty()){
			_open_file = filepath;
			save();
		}
	}

}
