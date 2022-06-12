#pragma once

#include "Window.h"

#include "ECS/Environment.h"
#include "ECS/Entity.h"
#include "assets/AssetManager.h"


namespace Phoenix{
	class FrameBuffer;
	class Renderer2D;
	class Renderer3D;
	// class AssetManager;

	// struct EngineConfig{
		// std::string name = "Phoenix Engine";
	// };

	struct WindowPerformanceMetrics{
		float renderLoop = 0;
		float draw = 0;

		float draw3D = 0;
		float render3D = 0;
		unsigned int drawCalls3D = 0;
		unsigned int verticies3D = 0;
		unsigned int indicies3D = 0;

		float draw2D = 0;
		float render2D = 0;
		unsigned int drawCalls2D = 0;
		unsigned int verticies2D = 0;
		unsigned int indicies2D = 0;
		
		float updateECS = 0;
		float renderECS = 0;
	};

	struct EnginePerformanceMetrics{
		float engineLoop = 0;

		std::unordered_map<winID, WindowPerformanceMetrics> windows;

		unsigned int entites = 0;
	};

	

	class Engine{
		public:
			Engine();
			// Engine(EngineConfig config);
			~Engine();

			void run();
			void exit();

			virtual void create(){};
			virtual void render3D(){};
			virtual void render2D(){};


			// window data
			winID createWindow(WindowConfig config);
			bool bindWindow(winID id);
			bool keyDown(winID id, keyCode keycode);
			bool mouseButtonDown(winID id, keyCode button);
			float mouseX(winID id);
			float mouseY(winID id);
			inline int windowWidth(winID id) { return _windows[id]->getWidth(); }
			inline int windowHeight(winID id) { return _windows[id]->getHeight(); }

			inline Window* getWindow(winID id) { return _windows[id]; }
			// void render();


			// ECS
			inline Environment* getEnvironment() const { return _environment; }
			inline void clearEnvironment(){ _environment->clear(); };
			Entity createEntity(const std::string& name = "Entity");
			Entity createEntity(const std::string& name, const UUID& uuid);
			inline void destroyEntity(Entity& entity){ _environment->destroyEntity(entity); };

			inline void	serialize(const std::string& filepath) const { _environment->serialize(filepath); }
			inline void	deserialize(const std::string& filepath) const { _environment->deserialize(filepath); }


			// rendering								
			inline void clearColor(){ 		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }
			inline void clearDepth(){ 		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_DEPTH_BUFFER_BIT); }
			inline void clearColorDepth(){ 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

			inline void enableDepthTest(){	glEnable(GL_DEPTH_TEST);  }
			inline void disableDepthTest(){	glDisable(GL_DEPTH_TEST); }

			void copyFrameBuffer(FrameBuffer* a, FrameBuffer* b);

			void setCamera(Entity camera);



			// asset manager
			inline UUID loadShader(std::string filepath){ return _asset_manager->loadShader(filepath); };
			inline void bindShader(UUID& uuid){ _asset_manager->bindShader(uuid); };

		public:
			EnginePerformanceMetrics performanceMetrics;


		private:
			// EngineConfig _config;
			bool _running = true;

			winID _window_id = 0;
			std::map<winID, Window*> _windows;

			Environment* _environment;
			Renderer2D* _renderer_2d;
			Renderer3D* _renderer_3d;
			AssetManager* _asset_manager;

		protected:
			Entity _camera{};

		private:
			void clear_perf_metrics();
			void set_perf_metrics(winID id);
			void set_perf_metrics();
	};


	// user initialization
	Engine* init();
	
}
