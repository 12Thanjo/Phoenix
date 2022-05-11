#include <Phoenix.h>
#include <core/init.h>

#include "ImGui.h"


namespace Phoenix{

	class Editor : public Engine{
		private:
			RendererImGui renderer_ImGui;
	
		public:
			Editor()
				: Engine({"Phoenix Editor", 1600, 900, true}), 
				renderer_ImGui(static_cast<Engine*>(this))
				{};


			void update() override{
				renderer_ImGui.begin();

				renderer_ImGui.render(static_cast<Engine*>(this));

				renderer_ImGui.end();
			}

	};


	Engine* init(){
		return new Editor();
	}
}

