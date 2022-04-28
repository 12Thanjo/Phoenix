

#include "ph_pch.h"

#include "Application.h"
#include "CMD.h"



#include <glad/glad.h>



namespace Phoenix{

	#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;

	Application::Application(){
		ph_internal_assert(!instance, "Application already exists");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->set_callback(BIND_EVENT_FUNCTION(Application::onEvent));

		// LayerStack* layers = new LayerStack();
	}

	Application::~Application(){
		// delete layers;
	}

	void Application::onEvent(Event& e){

		if(e.getType() == PH_WINDOW_CLOSE_EVENT){
			running = false;
		}


		for(Layer* layer : layers){
			layer->onEvent(e);
			if(e.handled){
				break;
			}
		}


		ph_internal_info(e);
	}


	void Application::run(){
		while(running){
			glClearColor(0, 0.06f, 0.13f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for(Layer* layer : layers){
				layer->onUpdate();
			}

			window->run();
		}
	}



	void Application::pushLayer(Layer* layer){
		layers.push_layer(layer);
	}

	void Application::pushOverlay(Layer* overlay){
		layers.push_overlay(overlay);
	}


	void Application::popLayer(Layer* layer){
		layers.pop_layer(layer);
	}

	void Application::popOverlay(Layer* overlay){
		layers.pop_overlay(overlay);
	}


}


