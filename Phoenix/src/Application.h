#pragma once


#include "Window.h"
#include "layers/LayerStack.h"
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
			LayerStack layers;

		public:
			Application();
			virtual ~Application();
			

			void run();
			void onEvent(Event& e);


			void pushLayer(Layer* layer);
			void pushOverlay(Layer* overlay);

			void popLayer(Layer* layer);
			void popOverlay(Layer* overlay);



			inline Application& get() const { return *instance; }
	};


	Application* initialize();

}





