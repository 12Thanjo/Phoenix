#pragma once
#include "Panel.h"


namespace Phoenix{
	
	class PerformancePanel : public Panel{
		public:
			PerformancePanel() = default;
			~PerformancePanel() = default;
	
			void render(Engine* editor) override;
			
	};

}
