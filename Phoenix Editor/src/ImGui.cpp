#include <Phoenix.h>
#include "ImGui.h"

#include "Application.h"
#include "ImGui helpers.h"

#include "Project.h"

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

		//////////////////////////////////////////////////////////////////////
		// icons
	}


	void RendererImGui::init(){
		ImGui_ImplGlfw_InitForOpenGL(_editor->getWindow()->getWindowContext(), true);
		ImGui_ImplOpenGL3_Init("#version 410");


		scene_hierarchy_panel = static_cast<Panel*>(new SceneHierarchyPanel());
		_panels.push_back(scene_hierarchy_panel);
		
		_panels.push_back(static_cast<Panel*>(new AssetBrowserPanel()));
		_panels.push_back(static_cast<Panel*>(new PerformancePanel()));
		_panels.push_back(static_cast<Panel*>(new ScenePanel()));




		camera_icon = _editor->loadTexture("assets/icons/cards/camera.png");
		orbital_camera_icon = _editor->loadTexture("assets/icons/cards/orbital camera.png");
		lightbulb_icon = _editor->loadTexture("assets/icons/cards/lightbulb.png");



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
				bool menu_item;
				#define HOVER_MENU_ITEM(title, msg, action) menu_item = ImGui::MenuItem(title, "..."); \
					imgui_hover(msg);\
					if(menu_item)action
				#define MENU_ITEM(title, command, action) if(ImGui::MenuItem(title, command))action

			    if(ImGui::BeginMenu("File")){
			    	if(ImGui::MenuItem("New", "Ctrl+N")){
			    		PH_FATAL("This is an unsupported feature");
			    		PH_TRACE();
			    	// }else if(ImGui::MenuItem("Open", "Ctrl+O")){
			    	// 	open();
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


			    if(ImGui::BeginMenu("Project")){

			    	HOVER_MENU_ITEM("New Empty Entity", "Right Click in Scene Hierarchy Panel", {
			    		_editor->createEntity("Empty Entity");
			    	});

			    	imgui_separator();

			    	MENU_ITEM("Run", "Ctrl+R", {
			    		static_cast<Editor*>(_editor)->runProject();
			    	});
			    	
			    	ImGui::EndMenu();
			    }


			    if(ImGui::BeginMenu("View")){
			    	if(ImGui::MenuItem("Camera Top View", "NUM7")){
			    		OrbitalCamera& camera = _editor->getScene()->camera;
			    		camera.setCoordinates(camera.getRho(), 1.5708f, 0);
			    	}else if(ImGui::MenuItem("Camera Bottom View", "NUM9")){
			    		OrbitalCamera& camera = _editor->getScene()->camera;
			    		camera.setCoordinates(camera.getRho(), 1.5708f, 3.1416f);
			    	}else if(ImGui::MenuItem("Camera Front View", "NUM1")){
			    		OrbitalCamera& camera = _editor->getScene()->camera;
			    		camera.setCoordinates(camera.getRho(), 1.5708f, 1.5708f);
			    	}else if(ImGui::MenuItem("Camera Side View", "NUM3")){
			    		OrbitalCamera& camera = _editor->getScene()->camera;
			    		camera.setCoordinates(camera.getRho(), 3.1416f, 1.5708f);
			    	}

			    	ImGui::EndMenu();
			    }

			    if(ImGui::MenuItem("Help")){
			    	imgui_start_alert("Commands and Controls",
R"(File:
	New Scene:                Ctrl+N
	Save Project and Scene:   Ctrl+S
	Save Scene as:            Ctrl+Shift+S

Project:
	Create New Empty Entity:  Right Click in Entity Panel
	Select Entity:            Click on Entity in the Viewport or
                                  in the Scene Hierarchy Panel
	Delete Entity:            Right Click in Scene Hierarchy
                                  Panel on specific entity
	Load Scene:               Drag from Asset Browser Panel
	                              into Viewport
	Run Project:              Ctrl+R

View:
	Camera Top View:          Numpad 7
	Camera Bottom View:       Numpad 9
	Camera Front View:        Numpad 1
	Camera Side View:         Numpad 3
	
Editor:
	Rotate Camera:            Middle Mouse or Space
	Pan Camera:               Shift+(Middle Mouse or Space)
	Zoom Camera:              Ctrl+(Middle Mouse or Space)
							      or Scroll Wheel

	Grab:                     G
	Rotate:                   R
	Scale:                    S

	Delimit X-axis:           X
	Deliminate X-axis:        Shift+X
	Delimit Y-axis:           Y
	Deliminate Y-axis:        Shift+Y
	Delimit Z-axis:           Z
	Deliminate Z-axis:        Shift+Z


Some UI buttons will show functionality details when the mouse
cursor is hovered over them.

)");			
			    }



			    ImGui::EndMenuBar();
			}



			// viewport
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
				std::string veiwport_title;
				if(_scene_state == SceneState::Edit){
					if(static_cast<SceneHierarchyPanel*>(scene_hierarchy_panel)->current_state == SHP_State::Grab){
						veiwport_title = "Viewport (grab)";
					}else if(static_cast<SceneHierarchyPanel*>(scene_hierarchy_panel)->current_state == SHP_State::Rotate){
						veiwport_title = "Viewport (rotate)";
					}else if(static_cast<SceneHierarchyPanel*>(scene_hierarchy_panel)->current_state == SHP_State::Scale){
						veiwport_title = "Viewport (scale)";
					}else if(static_cast<SceneHierarchyPanel*>(scene_hierarchy_panel)->current_state == SHP_State::None){
						veiwport_title = "Viewport";
					}else{
						veiwport_title = "Viewport [ERROR]";
					}
				}else if(_scene_state == SceneState::Play){
					veiwport_title = "Viewport (play)";
				}else{
					veiwport_title = "Viewport [ERROR]";
				}
				imgui_begin("viewport", veiwport_title);
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
							if(filesystem_path.extension() == ".phoenix_scene"){
								PH_LOG(filesystem_path.string());
								open_scene(filesystem_path.string());
								// open_scene(Files::getFilePath(static_cast<Editor*>(_editor)->project.path.string()) + "\\" + filesystem_path.string());
							}else{
								PH_WARNING("Attempted to drag & load a non-scene");
							}

						}

						// works without this, but I put it here just in case
						ImGui::EndDragDropTarget();
					}

					_mouse_over_viewport = ImGui::IsWindowHovered();

					ImVec2 viewport_size = ImGui::GetWindowSize();
					ImVec2 viewport_pos = ImGui::GetWindowPos();
					ImVec2 mouse_pos = ImGui::GetMousePos();

					int mouse_x = (int)(mouse_pos.x - viewport_pos.x);
					int mouse_y = (int)(mouse_pos.y - viewport_pos.y - (viewport_size.y - _viewport_size.y));


					_mouse_over_entity_id = render_buffer->readPixel(1, mouse_x, _viewport_size.y - mouse_y);
					

				ImGui::End();
			ImGui::PopStyleVar();




			// panels
			for(auto panel : _panels){
				panel->render(_editor);
			}

			// ImGui::ShowDemoWindow();


			// automatically run
			imgui_alert();


			ImGui::End();
		}

	}


	void RendererImGui::onEvent(Event& e){
		for(auto panel : _panels){
			panel->onEvent(e, _editor);
		}
	}




	//////////////////////////////////////////////////////////////////////
	// misc


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



	//////////////////////////////////////////////////////////////////////
	// scenes

	void RendererImGui::play_scene(){
		_scene_state = SceneState::Play;
		PH_LOG("Began Playing Scene");
	}

	void RendererImGui::stop_scene(){
		_scene_state = SceneState::Edit;
		PH_LOG("Stopped Playing Scene");
	}


	void RendererImGui::playEvent(){
		if(_scene_state == SceneState::Edit){
			play_scene();
		}else{
			stop_scene();
		}



		Files::isProcessRunning("C:\\Program Files\\Waterfox\\waterfox.exe");
	}




	//////////////////////////////////////////////////////////////////////
	// saving, loading, etc.


	void RendererImGui::newScene(){
		_editor->clearScene();
		_open_scene = std::string();
	}

	std::string RendererImGui::newProject(){
		return FileDialogs::save(*_editor->getWindow(), {
			.title = "Create Project",
			.filters = {
				{"Phoenix Project", ""}
			}
		});
	}

	std::string RendererImGui::open(Project& project){

		FileDialogsConfig config;
		config.filters = {
			{"Phoenix Project (*.phoenix)", "*.phoenix"}
		};

		if(!static_cast<Editor*>(_editor)->config.last_opened_project.empty()){
			config.filepath = static_cast<Editor*>(_editor)->config.last_opened_project;
		}
		

		std::string filepath = FileDialogs::open(*_editor->getWindow(), config);

		if(!filepath.empty()){
			std::string opened = open(filepath, project);

			if(!opened.empty()){
				return std::string();
			}

			open_scene(
				project.getRelativePath() + project.scenes.getLeft(project.getStartupScene())
			);
		}

		return filepath;
	}

	// void RendererImGui::open(std::string filepath){
	// 	_editor->clearScene();

	// 	std::string deserialize = _editor->deserialize(filepath);
	// 	if(deserialize.empty()){
	// 		_open_scene = filepath;
	// 		_just_opened = true;

	// 		PH_LOG("Opened Scene: " << _open_scene);
	// 	}else{
	// 		imgui_start_alert(deserialize);
	// 		newScene();
	// 	}
	// }
	

	std::string RendererImGui::open(const std::string& filepath, Project& project){
		std::string deserialize = project.deserialize(filepath, _editor);
		if(!deserialize.empty()){
			imgui_start_alert(deserialize);
		}
		return deserialize;
	}


	void RendererImGui::open_scene(const std::string& filepath){
		_editor->clearScene();
		static_cast<SceneHierarchyPanel*>(scene_hierarchy_panel)->current_state = SHP_State::None;
		static_cast<SceneHierarchyPanel*>(scene_hierarchy_panel)->selection_context = {};


		std::string deserialize = _editor->deserialize(filepath);
		if(deserialize.empty()){
			_open_scene = filepath;
			_just_opened = true;

			//////////////////////////////////////////////////////////////////////
			// adding cards
			_editor->getScene()->each<Component::PerspectiveCamera>([&](Entity entity, Component::PerspectiveCamera& camera){
				entity.addComponent<Component::Card>(camera_icon).scale = {0.4f, 0.4f};
			});

			_editor->getScene()->each<Component::OrbitalCamera>([&](Entity entity, Component::OrbitalCamera& camera){
				entity.addComponent<Component::Card>(orbital_camera_icon).scale = {0.4f, 0.4f};
			});

			_editor->getScene()->each<Component::PointLight>([&](Entity entity, Component::PointLight& camera){
				entity.addComponent<Component::Card>(lightbulb_icon).scale = {0.4f, 0.4f};
			});

			PH_LOG("Opened Scene: " << _open_scene);
		}else{
			imgui_start_alert(deserialize);
			newScene();
		}
	}


	void RendererImGui::save(){
		static_cast<Editor*>(_editor)->project.serialize();
		

		if(!_open_scene.empty()){
			_editor->serialize(_open_scene);

			PH_LOG("Saved:" << 
				"\n\tProject: " << static_cast<Editor*>(_editor)->project.path.string() << 
				"\n\tScene:   " << _open_scene);

#ifdef PH_DEBUG
		}else{
			PH_LOG("Saved:" << 
				"\n\tProject: " << static_cast<Editor*>(_editor)->project.path.string() << 
				"\n\tScene:   [none open]");
#endif
		}


	}

	void RendererImGui::save_as(){
		std::string filepath = FileDialogs::save(*_editor->getWindow(), {
			.filters = {
				{"Phoenix Scene (*.phoenix_scene)", "*.phoenix_scene"}
			},
			.filepath = static_cast<Editor*>(_editor)->project.getRelativePath() + "\\scenes"
		});

		if(!filepath.empty()){
			_editor->getScene()->uuid = UUID();
			_open_scene = filepath;
			save();
		}
	}

}
