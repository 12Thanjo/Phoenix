#pragma once


#include "Window.h"
#include "events/event.h"


#define PhoenixInitialize(Type) Type* app = new Type(); \
	app->run(); \
	delete app;

namespace Phoenix{
	class Application{
		private:
			std::unique_ptr<Window> window;
			bool running = true;
			static Application* instance;

		public:
			Application();
			virtual ~Application();
			

			void run();
			void onEvent(Event& e);


			inline Application& get() const { return *instance; }
	};


	Application* initialize();

}





