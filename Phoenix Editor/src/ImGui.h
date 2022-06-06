#pragma once

#include <Phoenix.h>

#include "panels/Panel.h"


namespace Phoenix{

	class RendererImGui{
		private:
			Engine* _editor;
			glm::vec2 _viewport_size;

			std::vector<Panel*> _panels;
	
		public:
			RendererImGui(Engine* engine);
			~RendererImGui();

			void init(winID id);

			void begin();
			void end();
			void render(FrameBuffer* render_buffer);


			void set_dark_theme();
			
	};

}
