#include "Application.h"



#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Phoenix{

	Editor::Editor() : renderer_ImGui(dynamic_cast<Engine*>(this)), project() {};
	Editor::~Editor(){
		delete _output_buffer;
	};


	void Editor::create(){
		// onEvent callback
		std::function<void(Event&)> event_callback = [&](Event& e){
			EventType type = e.getType();


			bool ctrl_down = keyDown(PH_KEY_LEFT_CONTROL) || keyDown(PH_KEY_RIGHT_CONTROL);
			bool shift_down = keyDown(PH_KEY_LEFT_SHIFT) || keyDown(PH_KEY_RIGHT_SHIFT);

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
							}else if(ctrl_down){
								renderer_ImGui.save();
							}
							break;
						case PH_KEY_N:
							if(ctrl_down){
								renderer_ImGui.newScene();
							}
							break;
						case PH_KEY_P:
							if(ctrl_down){
								renderer_ImGui.playEvent();
							}
							break;
					};
					break;
				case PH_MOUSE_MOVE_EVENT:
					/*if(renderer_ImGui.getMouseOverViewport())*/{
						float current_mouse_x = mouseX();
						float current_mouse_y = mouseY();

						if(mouseButtonDown(PH_MOUSE_MIDDLE)){
							float dx = current_mouse_x - _mouse_x;
							float dy = current_mouse_y - _mouse_y;

							if(shift_down){
								pan_camera(dx, dy);
							}else if(ctrl_down){
								zoom_camera(dy * 0.0025f);
							}else{
								rotate_camera(dx, dy);
							}
						}

						_mouse_x = current_mouse_x;
						_mouse_y = current_mouse_y;
					}
					break;
				case PH_MOUSE_SCROLL_EVENT:
					if(renderer_ImGui.getMouseOverViewport()){
						zoom_camera(static_cast<MouseScrollEvent&>(e).getY() * -0.1f);
					}
					break;
			}


		};


		WindowConfig config;
			config.width = WINDOW_WIDTH;
			config.height = WINDOW_HEIGHT;
			config.name = "Phoenix Engine Editor";
			config.eventCallback = event_callback;
		createWindow(config);


		maximize();



		///////////////////////////////////////////////////////////////////////////////////////////////////////



		FrameBufferConfig output_config;
			output_config.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };
			output_config.width = WINDOW_WIDTH;
			output_config.height = WINDOW_HEIGHT;
			// output_config.samples = 4;
		_output_buffer = new FrameBuffer(output_config);




		renderer_ImGui.init();
	}



	void Editor::render(){
		_output_buffer->bind();
		clearColorDepth();

		render3D();
		render2D();

		_output_buffer->unbind();
		
		renderer_ImGui.begin();
		renderer_ImGui.render(_output_buffer);
		renderer_ImGui.end();
	}





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
			camera.getTheta() + (dx * 0.00685f),
			camera.getPhi() - (dy * 0.00685f)
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




	Engine* init(){
		// EngineConfig config;
			// config.name = "Phoenix Engine Editor";

		return new Editor();
	}
	
}
