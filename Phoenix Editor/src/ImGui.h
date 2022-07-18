#pragma once

#include <Phoenix.h>

#include "panels/Panel.h"

namespace Phoenix{
	class Project;

	class RendererImGui{
		public:
			RendererImGui(Engine* engine);
			~RendererImGui();

			void init();

			void begin();
			void end();
			void render(FrameBuffer* render_buffer);
			void onEvent(Event& e);

			void set_dark_theme();

			void newScene();
			std::string newProject();
			std::string open(Project& project);
			std::string open(const std::string& filepath, Project& project);
			void open_scene(const std::string& filepath);
			void save();
			void save_as();

			void playEvent();


			inline bool getMouseOverViewport() const { return _mouse_over_viewport;  };
			inline int  getEntityIdMouseOver() const { return _mouse_over_entity_id; };


		public:
			Panel* scene_hierarchy_panel;
		

		private:
			void play_scene();
			void stop_scene();
			
		private:
			Engine* _editor;

			std::vector<Panel*> _panels;

			std::string _open_scene;
			bool _just_opened = true;

			bool _mouse_over_viewport = false;
			glm::vec2 _viewport_size;
			glm::vec2 _viewport_bounds[2];
			int _mouse_over_entity_id = -1;

			enum struct SceneState{
				Edit = 0,
				Play = 1
			};

			SceneState _scene_state = SceneState::Edit;

	};

}
