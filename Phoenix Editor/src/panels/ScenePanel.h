#pragma once

#include "Panel.h"

namespace Phoenix{
	
	class ScenePanel : public Panel{
		public:
			ScenePanel() = default;
			~ScenePanel() = default;
	
			void render(Engine* editor) override;
			
	};

}
