#pragma once

#include <Phoenix.h>
#include "ImGui.h"

namespace Phoenix{

	
	class Editor : public Engine{
		public:
			Editor();
			~Editor();
			
			void create() override;
			void render() override;

		public:
			RendererImGui renderer_ImGui;


		private:
			void pan_camera(float dx, float dy);
			void rotate_camera(float dx, float dy);
			void zoom_camera(float dy);
			void place_camera();
			void use_editor_camera();


		private:
			FrameBuffer* _output_buffer;

			float _mouse_x = 0;
			float _mouse_y = 0;
			
	};



}
