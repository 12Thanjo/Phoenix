#pragma once

#include <Phoenix.h>

#include "panels/Panel.h"


namespace Phoenix{

	class RendererImGui{
		public:
			RendererImGui(Engine* engine);
			~RendererImGui();

			void init(winID id);

			void begin();
			void end();
			void render(FrameBuffer* render_buffer, const winID& win_id);


			void set_dark_theme();

			void open(winID win_id);
			void save();
			void save_as(winID win_id);
			
		private:
			Engine* _editor;
			glm::vec2 _viewport_size;

			std::vector<Panel*> _panels;

			std::string _open_file;
	
	};

}
