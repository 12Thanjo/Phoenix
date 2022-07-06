#include "Runtime.h"



#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

namespace Phoenix{

	Game::Game(std::string file) 
		: project(), Engine(file) {

		path = Files::getFilePath(file);
	}

	Game::~Game(){
		delete _output_buffer;
	}


	void Game::create(){
		// onEvent callback
		std::function<void(Event&)> event_callback = [&](Event& e){
			EventType type = e.getType();


			bool ctrl_down = keyDown(PH_KEY_LEFT_CONTROL) || keyDown(PH_KEY_RIGHT_CONTROL);
			bool shift_down = keyDown(PH_KEY_LEFT_SHIFT) || keyDown(PH_KEY_RIGHT_SHIFT);

			switch(type){
				case PH_WINDOW_CLOSE_EVENT:
					exit();
					break;
				case PH_WINDOW_RESIZE_EVENT:
					resizeCameras(
						static_cast<WindowResizeEvent&>(e).getWidth(),
						static_cast<WindowResizeEvent&>(e).getHeight()
					);
					break;
			}


		};


		WindowConfig config;
			config.width = WINDOW_WIDTH;
			config.height = WINDOW_HEIGHT;
			config.name = "Phoenix Runtime";
			config.eventCallback = event_callback;
		createWindow(config);





		///////////////////////////////////////////////////////////////////////////////////////////////////////



		FrameBufferConfig output_config;
			output_config.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };
			output_config.width = WINDOW_WIDTH;
			output_config.height = WINDOW_HEIGHT;
			// output_config.samples = 4;
		_output_buffer = new FrameBuffer(output_config);


		project_path = Files::readFile(path + "\\project.phoenix_runtime_config");
		project_path = project_path.substr(0, project_path.length()-1);


		project.deserialize(project_path);
		deserialize(Files::getFilePath(project_path) + project.scenes.getLeft(project.getStartupScene()));



		Scene* scene = getScene();
		if(scene->hasStartupCamera()){
			setCamera(
				Entity{
					scene->getEntityByUUID(scene->getStartupCamera()),
					scene
				}
			);

			PH_LOG("set primary camera");
		}



		// maximize();
		getWindow()->fullscreen();

	}



	void Game::render(){
		// _output_buffer->bind();
		clearColorDepth();

		runScripts();

		render3D();
		render2D();


		// _output_buffer->unbind();

	}




	Engine* init(std::string path){
		// EngineConfig config;
			// config.name = "Phoenix Engine Game";

		return new Game(path);
	}
	
}
