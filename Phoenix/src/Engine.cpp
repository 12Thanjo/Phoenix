#include "ph_pch.h"
#include "Engine.h"


#define PERF_START(name) Timer _timer_##name
#define PERF_END(name) performanceMetrics.name = _timer_##name.stop()

namespace Phoenix{

	Engine::Engine(const WindowProperties& props){
		window = CreateScope<Window>(props);
		renderer = CreateRef<Renderer>(props.width, props.height);


		window->set_event_callback(BIND_FUNCTION(event_callback));
	}

	void Engine::run(){
		create();

		PH_INFO("Begining Render Loop");

		update();
		while(running){
			PERF_START(engineFrame);
				PERF_START(renderLoop);
					window->clear();

					PERF_START(draw);
						renderer->begin_scene();
						renderer->render();

						PERF_START(ECS);
							environment.update(renderer);
						PERF_END(ECS);
						
						renderer->end_scene();

						window->update();
					PERF_END(draw);


					PERF_START(update);
						update();
					PERF_END(update);

				PERF_END(renderLoop);

				update_perf_metrics();

			PERF_END(engineFrame);
		};
	}



	void Engine::event_callback(Event& e){
		EventType type = e.getType();
		if(type == PH_WINDOW_CLOSE_EVENT){
			running = false;
		}

		InputManager::onEvent(e);
	}


	void Engine::update_perf_metrics(){
		performanceMetrics.drawCalls = renderer->performanceMetrics.draw_calls;
		performanceMetrics.verticies = renderer->performanceMetrics.verticies;
		performanceMetrics.indicies = renderer->performanceMetrics.indicies;

		performanceMetrics.drawCalls2D = renderer->performanceMetrics.draw_calls_2d;
		performanceMetrics.quads = renderer->performanceMetrics.quads;
		performanceMetrics.verticies2D = renderer->performanceMetrics.verticies_2d;
		performanceMetrics.indicies2D = renderer->performanceMetrics.indicies_2d;

		performanceMetrics.entities = environment.performanceMetrics.entities;
	}
	
}



#undef PERF_START
#undef PERF_END