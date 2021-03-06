#include "Application.h"

#include "editor events.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Phoenix{

	
	Editor::~Editor(){
		delete _output_buffer;
	};


	void Editor::create(){
		//////////////////////////////////////////////////////////////////////
		// config file

		std::string appdata = Files::getEnvVar("APPDATA");
		PH_ASSERT(!appdata.empty(), "Error retrieving appdata directory");
		
		std::string config_file_string;

		appdata += "\\Phoenix Engine";

		if(!Files::directoryExists(appdata)){
			Files::createDirectory(appdata, [&](std::string e){
				PH_FATAL(e);
			});

			PH_LOG("Created APPDATA folder");

			config_file_string = saveConfig();
		}else if(!Files::fileExists(appdata + "\\config.naml")){
			config_file_string = saveConfig();
		}else{
			config_file_string = Files::readFile(appdata + "\\config.naml");
		}

		
		NAML_DE naml{config_file_string};
		config.VSync = naml.get()->get("VSync")->value<bool>();
		if(naml.get()->has("Last Opened Project")){
			config.last_opened_project = naml.get()->get("Last Opened Project")->value<std::string>();
		}


		//////////////////////////////////////////////////////////////////////
		// onEvent callback

		std::function<void(Event&)> event_callback = [&](Event& e){
			EventType type = e.getType();
			bool captured = false;
			bool send_event = false;
			Event* event_to_send;


			bool ctrl_down = keyDown(PH_KEY_LEFT_CONTROL) || keyDown(PH_KEY_RIGHT_CONTROL);
			bool shift_down = keyDown(PH_KEY_LEFT_SHIFT) || keyDown(PH_KEY_RIGHT_SHIFT);
			bool alt_down = keyDown(PH_KEY_LEFT_ALT) || keyDown(PH_KEY_RIGHT_ALT);


			#define SEND_EVENT(x) event_to_send = new x(); send_event = true;
			#define BREAK_CAPTURE() captured = true; break;
			#define CAPTURE() captured = true;

			switch(type){
				case PH_WINDOW_CLOSE_EVENT:
					exit();
					break;
				case PH_KEY_DOWN_EVENT:
					switch(static_cast<KeyDownEvent&>(e).getKeycode()){
						// case PH_KEY_O:
						// 	if(ctrl_down){
						// 		renderer_ImGui.open();
						// 	}
						// 	break;
						case PH_KEY_S:
							if(ctrl_down && shift_down){
								renderer_ImGui.save_as();
								CAPTURE();
							}else if(ctrl_down){
								renderer_ImGui.save();
								CAPTURE();
							}else{
								if(renderer_ImGui.getMouseOverViewport()){
									SEND_EVENT(ScaleEvent);
								}
							}

							break;
						case PH_KEY_N:
							if(ctrl_down){
								renderer_ImGui.newScene();
							}

							BREAK_CAPTURE();
						case PH_KEY_F5:
							PH_WARNING("I don't do much yet");
							renderer_ImGui.playEvent();

							BREAK_CAPTURE();
						case PH_KEY_R:
							if(ctrl_down){
								runProject();

								CAPTURE();
							}else{
								if(renderer_ImGui.getMouseOverViewport()){
									SEND_EVENT(RotateEvent);
								}
							}
							break;
						case PH_KEY_G:
							if(renderer_ImGui.getMouseOverViewport()){
								SEND_EVENT(GrabEvent);
							}
							break;

						//////////////////////////////////////////////////////////////////////
						// camera position
						case PH_KEY_NUMPAD_7:
							if(renderer_ImGui.getMouseOverViewport()){
								OrbitalCamera& camera = _scene->camera;
								camera.setCoordinates(camera.getRho(), 1.5708f, 0);
							}
							break;
						case PH_KEY_NUMPAD_9:
							if(renderer_ImGui.getMouseOverViewport()){
								OrbitalCamera& camera = _scene->camera;
								camera.setCoordinates(camera.getRho(), 1.5708f, 3.1416f);
							}
							break;
						case PH_KEY_NUMPAD_1:
							if(renderer_ImGui.getMouseOverViewport()){
								OrbitalCamera& camera = _scene->camera;
								camera.setCoordinates(camera.getRho(), 1.5708f, 1.5708f);
							}
							break;
						case PH_KEY_NUMPAD_3:
							if(renderer_ImGui.getMouseOverViewport()){
								OrbitalCamera& camera = _scene->camera;
								camera.setCoordinates(camera.getRho(), 3.1416f, 1.5708f);
							}
							break;
					};
					break;
				case PH_MOUSE_MOVE_EVENT:
					/*if(renderer_ImGui.getMouseOverViewport())*/{
						float current_mouse_x = mouseX();
						float current_mouse_y = mouseY();

						if(mouseButtonDown(PH_MOUSE_MIDDLE) || keyDown(PH_KEY_SPACE)){
							float dx = current_mouse_x - _mouse_x;
							float dy = current_mouse_y - _mouse_y;

							if(shift_down){
								pan_camera(dx, dy);
							}else if(ctrl_down){
								zoom_camera(dy * 0.0025f);
							}else{
								rotate_camera(dx, dy);
							}
							CAPTURE();
						}

						_mouse_x = current_mouse_x;
						_mouse_y = current_mouse_y;
					}
					break;
				case PH_MOUSE_SCROLL_EVENT:
					if(renderer_ImGui.getMouseOverViewport()){
						zoom_camera(static_cast<MouseScrollEvent&>(e).getY() * -0.1f);
						CAPTURE();
					}
					break;
			}


			if(send_event){
				renderer_ImGui.onEvent(*event_to_send);
			}else if(!captured){
				renderer_ImGui.onEvent(e);
			}
		};



		//////////////////////////////////////////////////////////////////////
		// window and frame buffer


		WindowConfig win_config;
			win_config.width = WINDOW_WIDTH;
			win_config.height = WINDOW_HEIGHT;
			win_config.name = "Phoenix Engine Editor";
			win_config.eventCallback = event_callback;
			win_config.VSync = config.VSync;
			win_config.backfaceCulling = true;
		createWindow(win_config);


		getWindow()->maximize();

		FrameBufferConfig output_config;
			output_config.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
			output_config.width = WINDOW_WIDTH;
			output_config.height = WINDOW_HEIGHT;
			// output_config.samples = 4;
		_output_buffer = new FrameBuffer(output_config);


		

		//////////////////////////////////////////////////////////////////////
		// imgui

		renderer_ImGui.init();
	}



	void Editor::render(){
		if(!_can_render){
			_running_counter += 1;
			if(_running_counter > 100){
				_can_render = !Files::isProcessRunning(Files::getFilePathUpDirectory(_path) + "/Phoenix Runtime/Phoenix Runtime.exe");
				_running_counter = 0;
			}
		}


		if(_can_render){
			_output_buffer->bind();
			clearColorDepth();
			_output_buffer->clearAttachment(1, -1);

			render3D();
			render2D();

			_output_buffer->unbind();

			
			renderer_ImGui.begin();
			renderer_ImGui.render(_output_buffer);
			renderer_ImGui.end();
		}
	}




	void Editor::runProject(){
		std::string runtime_path = Files::getFilePathUpDirectory(_path) + "\\Phoenix Runtime\\Phoenix Runtime.exe";
		PH_ASSERT(Files::fileExists(runtime_path), "Incorrect File Path for runtime");

		Files::writeFile(Files::getFilePath(runtime_path) + "\\project.phoenix_runtime_config", project.path.string());
		
		renderer_ImGui.save();
		Files::openInDefaultProgram(runtime_path);
		_can_render = false;
	}



	//////////////////////////////////////////////////////////////////////
	// camera controls

	void Editor::pan_camera(float dx, float dy){
		use_editor_camera();

		OrbitalCamera& camera = _scene->camera;
		glm::vec3 rotation = camera.getRotation();
		float ammount = camera.getRho() * 0.0015f;


		glm::vec3 focal_point = camera.getFocalPoint();


		focal_point.z -= dx * ammount * cos(rotation.y);
		focal_point.x += dx * ammount * sin(rotation.y);

		focal_point.y += dy * ammount * cos(rotation.z);
		focal_point.x -= dy * ammount * sin(rotation.z) * cos(rotation.y);
		focal_point.z -= dy * ammount * sin(rotation.z) * sin(rotation.y);


		camera.setFocalPoint(focal_point);

	}

	void Editor::rotate_camera(float dx, float dy){
		use_editor_camera();

		OrbitalCamera& camera = _scene->camera;

		camera.setCoordinates(
			camera.getRho(),
			fmod(camera.getTheta(), 6.283185307179586f) + (dx * 0.00685f),
			fmod(camera.getPhi(), 6.283185307179586f) - (dy * 0.00685f)
		);
	}

	void Editor::zoom_camera(float dy){
		use_editor_camera();
		
		OrbitalCamera& camera = _scene->camera;
	
		camera.setCoordinates(
			camera.getRho() * (dy + 1),
			camera.getTheta(),
			camera.getPhi()
		);
	}


	void Editor::use_editor_camera(){
		if(_camera){
			_camera = {};
		}
	}


	std::string Editor::saveConfig(){
		std::string appdata = Files::getEnvVar("APPDATA");
		PH_ASSERT(!appdata.empty(), "Error retrieving appdata directory");

		appdata += "\\Phoenix Engine";

		NAML_S serializer{};
		serializer.keyValue("VSync", config.VSync);

		if(!config.last_opened_project.empty()){
			serializer.keyValue("Last Opened Project", config.last_opened_project);
		}

		std::string config_file_string = serializer.output();

		Files::writeFile(appdata + "\\config.naml", config_file_string.c_str());

		PH_LOG("Created Editor Config File");

		return config_file_string;
	}




	Engine* init(std::string path){
		// EngineConfig config;
			// config.name = "Phoenix Engine Editor";

		return new Editor(path);
	}
	
}
