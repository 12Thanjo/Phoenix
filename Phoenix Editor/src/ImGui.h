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

			void set_dark_theme();

			void newScene();
			std::string newProject();
			std::string open(Project& project);
			std::string open(const std::string& filepath, Project& project);
			void open_scene(const std::string& filepath);
			void save();
			void save_as();
			
		private:
			Engine* _editor;
			glm::vec2 _viewport_size;

			std::vector<Panel*> _panels;

			std::string _open_scene;

			bool _just_opened = true;
	};

}
