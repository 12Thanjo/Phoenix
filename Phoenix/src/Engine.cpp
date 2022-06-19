#include "ph_pch.h"
#include "Engine.h"

#include "rendering/FrameBuffer.h"
#include "rendering/Renderer2D.h"
#include "rendering/Renderer3D.h"
#include "render objects/cameras.h"
#include "ECS/components.h"



#define PERF_START(name) Timer _timer_##name
#define PERF_END(name) performanceMetrics.name = (float)_timer_##name.stop()


namespace Phoenix{
	
	Engine::Engine(){
		_scene = new Scene();
		_asset_manager = new AssetManager();
		_renderer_2d = new Renderer2D(_asset_manager);
		_renderer_3d = new Renderer3D(_asset_manager);
		// _window = new Window();
	}

	// Engine::Engine(EngineConfig config)
	// 	: _config(config) {

	// }


	Engine::~Engine(){
	   	delete _window;

	   	glfwTerminate();

	   	delete _scene;
	   	delete _renderer_2d;
	   	delete _renderer_3d;
	   	delete _asset_manager;
	};



	void Engine::run(){
		create();
		_renderer_2d->init();
		_renderer_3d->init();
		PH_INFO("Created Engine");


		glEnable(GL_BLEND);
		// glDepthFunc(GL_LEQUAL);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		while(_running){
			PERF_START(engineLoop);
				PERF_START(renderLoop);
					_window->bind();

					PERF_START(updateECS);
						_scene->update();
					PERF_END(updateECS);
					
					PERF_START(draw);
						glEnable(GL_DEPTH_TEST);
						glClearColor(PH_COOL_GRAY_900);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						PERF_START(render3D);
							render3D();
						PERF_END(render3D);
						PERF_START(renderECS);

							Camera* camera;
							if(_camera){
								if(_camera.hasComponent<Component::PerspectiveCamera>()){
									camera = &_camera.getComponent<Component::PerspectiveCamera>().camera;
								}else{
									camera = &_camera.getComponent<Component::OrbitalCamera>().camera;	
								};
							}else{
								camera = &_scene->camera;	
							}

							PERF_START(draw3D);
								_scene->render3D(_renderer_3d, *camera);
							PERF_END(draw3D);
							PERF_START(draw2D);
								_scene->render2D(_renderer_2d, *camera);
							PERF_END(draw2D);

						PERF_END(renderECS);
						PERF_START(render2D);
							render2D();
						PERF_END(render2D);

						_window->render();
					PERF_END(draw);
				PERF_END(renderLoop);

				set_perf_metrics();

				clear_perf_metrics();

				_window->pollEvents();
		   	PERF_END(engineLoop);
		};
	}



	void Engine::createWindow(WindowConfig config){
		_window = new Window(config);
	}


	void Engine::exit(){
		_running = false;
	}


	bool Engine::keyDown(keyCode key){
		return _window->keyDown(key);
	}

	bool Engine::mouseButtonDown(keyCode button){
		return _window->mouseButtonDown(button);
	}

	float Engine::mouseX(){
		return _window->mouseX();
	}
	float Engine::mouseY(){
		return _window->mouseY();
	}


	//////////////////////////////////////////////////////////////////////
	// entities
	Entity Engine::createEntity(const std::string& name){
		return _scene->createEntity(name);
	}

	Entity Engine::createEntity(const std::string& name, const UUID& uuid){
		return _scene->createEntity(name, uuid);
	}


	//////////////////////////////////////////////////////////////////////
	// rendering
	void Engine::copyFrameBuffer(FrameBuffer* a, FrameBuffer* b){
		glBindFramebuffer(GL_READ_FRAMEBUFFER, a->getID());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, b->getID());
		glBlitFramebuffer(0, 0, a->getWidth(), a->getHeight(), 0, 0, b->getWidth(), b->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
	}
	


	void Engine::setCamera(Entity camera){
		_camera = camera;
	}



	//////////////////////////////////////////////////////////////////////
	// perf metrics

	// clears perf metrics held by renderers etc.
	void Engine::clear_perf_metrics(){
		_renderer_2d->resetPerfMetrics();
		_renderer_3d->resetPerfMetrics();
	}


	void Engine::set_perf_metrics(){
		performanceMetrics.entites = _scene->performanceMetrics.entities;


		performanceMetrics.drawCalls2D = _renderer_2d->performanceMetrics.drawCalls;
		performanceMetrics.verticies2D = _renderer_2d->performanceMetrics.verticies;
		performanceMetrics.indicies2D = _renderer_2d->performanceMetrics.indicies;

		performanceMetrics.drawCalls3D = _renderer_3d->performanceMetrics.drawCalls;
		performanceMetrics.verticies3D = _renderer_3d->performanceMetrics.verticies;
		performanceMetrics.indicies3D = _renderer_3d->performanceMetrics.indicies;
	}

}


#undef PERF_START
#undef PERF_START_WIN
#undef PERF_END