#include "ph_pch.h"
#include "Engine.h"

#include "rendering/FrameBuffer.h"
#include "rendering/Renderer2D.h"
#include "rendering/Renderer3D.h"
#include "render objects/cameras.h"
#include "ECS/components.h"



#define PERF_START(name) Timer _timer_##name
#define PERF_END(name) performanceMetrics.name = (float)_timer_##name.stop()
#define PERF_END_WIN(name) performanceMetrics.windows[id].name = (float)_timer_##name.stop()


namespace Phoenix{
	
	Engine::Engine(){
		_environment = new Environment();
		_asset_manager = new AssetManager();
		_renderer_2d = new Renderer2D(_asset_manager);
		_renderer_3d = new Renderer3D(_asset_manager);
	}

	// Engine::Engine(EngineConfig config)
	// 	: _config(config) {

	// }


	Engine::~Engine(){
		for(std::map<winID, Window*>::iterator itr = _windows.begin(); itr != _windows.end(); itr++){
	       delete itr->second;
	   	}

	   	glfwTerminate();

	   	delete _environment;
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
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		while(_running){
			PERF_START(engineLoop);
				for(std::map<winID, Window*>::iterator itr = _windows.begin(); itr != _windows.end(); itr++){
					Window* window = itr->second;
					winID id = window->getID();


					PERF_START(renderLoop);
						window->bind();

						PERF_START(updateECS);
							_environment->update();
						PERF_END_WIN(updateECS);
						
						PERF_START(draw);
							glEnable(GL_DEPTH_TEST);
							glClearColor(PH_COOL_GRAY_900);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

							PERF_START(render3D);
								render3D();
							PERF_END_WIN(render3D);
							PERF_START(renderECS);
								if(_camera){
									Camera& camera = _camera.getComponent<Component::Camera>().camera;

									PERF_START(draw3D);
										_environment->render3D(_renderer_3d, camera);
									PERF_END_WIN(draw3D);
									PERF_START(draw2D);
										_environment->render2D(_renderer_2d, camera);
									PERF_END_WIN(draw2D);
								}
							PERF_END_WIN(renderECS);
							PERF_START(render2D);
								render2D();
							PERF_END_WIN(render2D);

							window->render();
						PERF_END_WIN(draw);
					PERF_END_WIN(renderLoop);

					set_perf_metrics();
					set_perf_metrics(id);
					clear_perf_metrics();

					window->pollEvents();
			   	}
		   	PERF_END(engineLoop);
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

		performanceMetrics.windows[id] = WindowPerformanceMetrics();

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

	Entity Engine::createEntity(const std::string& name, const UUID& uuid){
		return _environment->createEntity(name, uuid);
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



	//////////////////////////////////////////////////////////////////////
	// perf metrics

	// clears perf metrics held by renderers etc.
	void Engine::clear_perf_metrics(){
		_renderer_2d->resetPerfMetrics();
		_renderer_3d->resetPerfMetrics();
	}

	void Engine::set_perf_metrics(winID id){
		WindowPerformanceMetrics& win_perf_metrics = performanceMetrics.windows[id];


		win_perf_metrics.drawCalls2D = _renderer_2d->performanceMetrics.drawCalls;
		win_perf_metrics.verticies2D = _renderer_2d->performanceMetrics.verticies;
		win_perf_metrics.indicies2D = _renderer_2d->performanceMetrics.indicies;

		win_perf_metrics.drawCalls3D = _renderer_3d->performanceMetrics.drawCalls;
		win_perf_metrics.verticies3D = _renderer_3d->performanceMetrics.verticies;
		win_perf_metrics.indicies3D = _renderer_3d->performanceMetrics.indicies;
	}

	void Engine::set_perf_metrics(){
		performanceMetrics.entites = _environment->performanceMetrics.entities;
	}

}


#undef PERF_START
#undef PERF_START_WIN
#undef PERF_END