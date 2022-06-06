#include "ph_pch.h"
#include "Engine.h"

#include "rendering/FrameBuffer.h"
#include "rendering/Renderer2D.h"
#include "render objects/cameras.h"
#include "ECS/components.h"


namespace Phoenix{
	
	Engine::Engine(){
		_environment = new Environment();
		_asset_manager = new AssetManager();
		_renderer_2d = new Renderer2D(_asset_manager);
	}

	// Engine::Engine(EngineConfig config)
	// 	: _config(config) {

	// }


	Engine::~Engine(){
		for(std::map<winID, Window*>::iterator itr = _windows.begin(); itr != _windows.end(); itr++){
	       delete itr->second;
	   	}

	   	glfwTerminate();

	   	free(_renderer_2d);
	   	free(_environment);
	   	free(_asset_manager);
	};



	void Engine::run(){
		create();
		_renderer_2d->init();
		PH_INFO("Created Engine");


		glEnable(GL_BLEND);
		glDepthFunc(GL_LEQUAL);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		while(_running){
			for(std::map<winID, Window*>::iterator itr = _windows.begin(); itr != _windows.end(); itr++){
				Window* window = itr->second;

				window->bind();

				_environment->update();
				
				glEnable(GL_DEPTH_TEST);
				glClearColor(PH_COOL_GRAY_900);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				render3D();
				if(_camera){
					_environment->render(_renderer_2d, _camera.getComponent<Component::Camera>().camera);
				}
				render2D();

				window->render();
				window->pollEvents();
		   	}
		};
	}



	winID Engine::createWindow(WindowConfig config){
		winID id = _window_id;
		_window_id += 1;

		if(id == 0){
			_windows[id] = new Window(id, config, nullptr);
		}else{
			_windows[id] = new Window(id, config, _windows[0]->getWindowContext());
		}

		return id;
	}


	bool Engine::bindWindow(winID id){
		if(_windows[id]){
			_windows[id]->bind();
			return true;
		}

		return false;
	}



	// void Engine::window_stuffs(winID id){
	// 	PH_ASSERT(_bound_window, "No window is bound");
	// }


	void Engine::exit(){
		_running = false;
	}


	bool Engine::keyDown(winID id, keyCode key){
		return _windows[id]->keyDown(key);
	}

	bool Engine::mouseButtonDown(winID id, keyCode button){
		return _windows[id]->mouseButtonDown(button);
	}


	//////////////////////////////////////////////////////////////////////
	// entities
	Entity Engine::createEntity(const std::string& name){
		return _environment->createEntity(name);
	}


	//////////////////////////////////////////////////////////////////////
	// rendering
	void Engine::copyFrameBuffer(FrameBuffer* a, FrameBuffer* b){
		glBindFramebuffer(GL_READ_FRAMEBUFFER, a->getID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, b->getID());
		glBlitFramebuffer(0, 0, a->getWidth(), a->getHeight(), 0, 0, b->getWidth(), b->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	


	void Engine::setCamera(Entity camera){
		if(_camera){
			_camera.getComponent<Component::Camera>().primary = false;
		}
		_camera = camera;
		_camera.getComponent<Component::Camera>().primary = true;
	}
}
