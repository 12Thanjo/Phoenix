#pragma once

#include "Window.h"

#include "ECS/Environment.h"
#include "ECS/Entity.h"
#include "assets/AssetManager.h"


namespace Phoenix{
	class FrameBuffer;
	class Renderer2D;
	// class AssetManager;

	// struct EngineConfig{
		// std::string name = "Phoenix Engine";
	// };


	class Engine{
		public:
			Engine();
			// Engine(EngineConfig config);
			~Engine();


			virtual void create(){};
			virtual void render3D(){};
			virtual void render2D(){};
			void run();
			

			winID createWindow(WindowConfig config);
			bool bindWindow(winID id);

			void exit();

			// window data
			bool keyDown(winID id, keyCode keycode);
			bool mouseButtonDown(winID id, keyCode button);
			inline int windowWidth(winID id) { return _windows[id]->getWidth(); }
			inline int windowHeight(winID id) { return _windows[id]->getHeight(); }

			inline Window* getWindow(winID id) { return _windows[id]; }
			// void render();


			// ECS
			Entity createEntity(const std::string& name = "Entity");
			inline Environment* getEnvironment() const { return _environment; }
			inline void	serialize(const std::string& filepath) const { _environment->serialize(filepath); }
			inline void	deserialize(const std::string& filepath) const { _environment->deserialize(filepath); }


			// rendering								
			inline void clearColor(){ 		glClearColor(PH_COOL_GRAY_900); glClear(GL_COLOR_BUFFER_BIT); }
			inline void clearDepth(){ 		glClearColor(PH_COOL_GRAY_900); glClear(GL_DEPTH_BUFFER_BIT); }
			inline void clearColorDepth(){ 	glClearColor(PH_COOL_GRAY_900); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

			inline void enableDepthTest(){	glEnable(GL_DEPTH_TEST);  }
			inline void disableDepthTest(){	glDisable(GL_DEPTH_TEST); }

			void copyFrameBuffer(FrameBuffer* a, FrameBuffer* b);

			void setCamera(Entity camera);



			// asset manager
			inline UUID loadShader(std::string filepath){ return _asset_manager->loadShader(filepath); };
			inline void bindShader(UUID& uuid){ _asset_manager->bindShader(uuid); };




		private:
			// EngineConfig _config;
			bool _running = true;

			winID _window_id = 0;
			std::map<winID, Window*> _windows;

			Environment* _environment;
			Renderer2D* _renderer_2d;
			AssetManager* _asset_manager;

			Entity _camera{};
	};


	// user initialization
	Engine* init();
	
}
