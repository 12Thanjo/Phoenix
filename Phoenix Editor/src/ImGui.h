#pragma once

#include <Phoenix.h>
#include "panels/panel.h"

namespace Phoenix{
	
	class RendererImGui{
		private:
			Engine* editor;

			glm::vec2 viewport_size;

			std::vector<Panel*> panels;
	
		public:
			RendererImGui(Engine* editor);
			~RendererImGui();


			void begin();
			void end();

			void render(Engine* editor);

			void set_dark_theme();
			
	};

	
}
