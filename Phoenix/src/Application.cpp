

#include "ph_pch.h"

#include "Application.h"
#include "CMD.h"



#include <GLFW/glfw3.h>



namespace Phoenix{

	#define BIND_EVENT_FUNCTION(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::instance = nullptr;

	Application::Application(){
		
		ph_internal_assert(!instance, "Application already exists");
		instance = this;

		window = std::unique_ptr<Window>(Window::create());
		window->set_callback(BIND_EVENT_FUNCTION(Application::onEvent));
	}

	Application::~Application(){

	}

	void Application::onEvent(Event& e){
		ph_internal_info("onEvent");
	}


	void Application::run(){
		while(running){
			window->run();
		}
	}

}


