#pragma once

#include "layers/Layer.h"

namespace Phoenix{
	class ImGuiLayer : public Layer{
		private:
			float time = 0.0f;
	
		public:
			ImGuiLayer();
			~ImGuiLayer();

			void onAttatch() override;
			void onDetatch() override;
			void onImGuiRender() override;



			void begin();
			void end();
	};
}
