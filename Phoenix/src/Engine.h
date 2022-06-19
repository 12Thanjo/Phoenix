#pragma once

#include "Window.h"

#include "ECS/Scene.h"
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



	struct EnginePerformanceMetrics{
		float engineLoop = 0;

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
			virtual void render(){};
			void render3D();
			void render2D();


			// window data
			void createWindow(WindowConfig config);
			// bool bindWindow(winID id);
			bool keyDown(keyCode keycode);
			bool mouseButtonDown(keyCode button);
			float mouseX();
			float mouseY();
			inline int windowWidth() { return _window->getWidth(); }
			inline int windowHeight() { return _window->getHeight(); }

			inline Window* getWindow() { return _window; }
			// void render();


			// ECS
			inline Scene* getScene() const { return _scene; }
			inline void clearScene(){ _scene = new Scene(); };
			Entity createEntity(const std::string& name = "Entity");
			Entity createEntity(const std::string& name, const UUID& uuid);
			inline void destroyEntity(Entity& entity){ _scene->destroyEntity(entity); };

			inline void	serialize(const std::string& filepath) const { _scene->serialize(filepath); }
			inline bool	deserialize(const std::string& filepath) const { return _scene->deserialize(filepath); }


			// rendering								
			inline void clearColor(){ 		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }
			inline void clearDepth(){ 		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_DEPTH_BUFFER_BIT); }
			inline void clearColorDepth(){ 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

			inline void enableDepthTest(){	glEnable(GL_DEPTH_TEST);  }
			inline void disableDepthTest(){	glDisable(GL_DEPTH_TEST); }

			void copyFrameBuffer(FrameBuffer* a, FrameBuffer* b);

			void setCamera(Entity camera);
			inline bool usingCamera() const { return _camera; };
			inline bool usingCamera(Entity camera) const { return camera == _camera; };


			// asset manager
			inline UUID loadShader(std::string filepath){ return _asset_manager->loadShader(filepath); };
			inline void bindShader(UUID& uuid){ _asset_manager->bindShader(uuid); };

		public:
			EnginePerformanceMetrics performanceMetrics;


		private:
			// EngineConfig _config;
			bool _running = true;

			Window* _window;
			bool _created_window = false;

			Renderer2D* _renderer_2d;
			Renderer3D* _renderer_3d;
			AssetManager* _asset_manager;

		protected:
			Scene* _scene;
			Entity _camera{};

		private:
			void clear_perf_metrics();
			void set_perf_metrics();
	};


	// user initialization
	Engine* init();
	
}
