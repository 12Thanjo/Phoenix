#pragma once

#include <Phoenix.h>

#include "panels/Panel.h"


namespace Phoenix{

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
			void open();
			void open(std::string filepath);
			void save();
			void save_as();
			
		private:
			Engine* _editor;
			glm::vec2 _viewport_size;

			std::vector<Panel*> _panels;

			std::string _open_file;

			bool _just_opened = true;
	};

}
