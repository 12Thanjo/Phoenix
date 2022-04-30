#pragma once


#include "Window.h"
#include "layers/LayerStack.h"
#include "events/event.h"

#include "layers/layers/ImGuiLayer.h"

#include "platform/OpenGL/OpenGLShader.h"
#include "renderer/buffer.h"



namespace Phoenix{
	class Application{
		private:
			std::unique_ptr<Window> window;
			ImGuiLayer* ImGui_layer;
			bool running = true;
			static Application* instance;
			LayerStack layers;

			unsigned int vertex_array;
			std::unique_ptr<OpenGLShader> shader;
			std::unique_ptr<VertexBuffer> vertex_buffer;
			std::unique_ptr<IndexBuffer> index_buffer;

		public:
			Application();
			virtual ~Application();
			

			void run();
			void onEvent(Event& e);


			void pushLayer(Layer* layer);
			void pushOverlay(Layer* overlay);

			void popLayer(Layer* layer);
			void popOverlay(Layer* overlay);



			inline static Application& get() { return *instance; }
			inline Window& getWindow() { return *window; }
	};


	Application* initialize();

}





