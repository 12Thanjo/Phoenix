#pragma once

#include "rendering/renderers/Renderer.h"
#include "rendering/window.h"

#include "events/events.h"
#include "InputManager.h"

#include "ECS/Environment.h"
#include "ECS/Entity.h"


namespace Phoenix{

	struct EnginePerformanceMetrics{
		float engineFrame = 0;
		float renderLoop = 0;
		float draw = 0;
		float ECS = 0;
		float update = 0;


		uint32_t drawCalls = 0;
		uint32_t verticies = 0;
		uint32_t indicies = 0;

		uint32_t drawCalls2D = 0;
		uint32_t quads = 0;
		uint32_t verticies2D = 0;
		uint32_t indicies2D = 0;

		uint32_t entities = 0;
	};

	class Engine{
		private:
			Scope<Window> window;
			bool running = true;
			Ref<Renderer> renderer;
			Environment environment;
			
		public:
			EnginePerformanceMetrics performanceMetrics;

			Engine(const WindowProperties& props = WindowProperties());


			inline unsigned int getWidth() const { return window->get_width(); }
			inline unsigned int getHeight() const { return window->get_height(); }
			inline void close(){ running = false; }

		public:

			virtual void create(){};
			virtual void update(){};
			virtual void onEvent(){};

			void run();


			inline GLFWwindow* getNativeWindow() const { return window->get_native_window(); }
			inline RendererID getFrameBufferColorAttachment() const { return window->get_frame_buffer_color_attachment(); }
			void resize(uint32_t width, uint32_t height){
				window->resize_frame_buffer(width, height);
				renderer->set_camera_projection((float)width, (float)height);
			}


			// ECS
			Entity createEntity(){ return environment.create_entity(); }
			Entity createEntity(const std::string& name){ return environment.create_entity(name); }
			void destroyEntity(Entity entity){ environment.destroy_entity(entity); }
			Environment& _get_environment(){ return environment; };


		private:
			void event_callback(Event& e);
			void update_perf_metrics();
			
	};


	// user initialization
	Engine* init();

}



