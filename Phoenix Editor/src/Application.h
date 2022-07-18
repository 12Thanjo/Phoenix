
#pragma once

#include <Phoenix.h>
#include "ImGui.h"

#include "Project.h"

namespace Phoenix{

	struct EditorConfig{
		bool VSync = false;
		std::string last_opened_project;
	};

	
	class Editor : public Engine{
		public:
			Editor(std::string path) : 
				Engine(path), renderer_ImGui(dynamic_cast<Engine*>(this)), project(), _path(Files::getFilePath(path)) {};
			~Editor();
			
			void create() override;
			void render() override;

			void runProject();

			std::string saveConfig();

		public:
			RendererImGui renderer_ImGui;
			Project project;
			EditorConfig config;


		private:
			void pan_camera(float dx, float dy);
			void rotate_camera(float dx, float dy);
			void zoom_camera(float dy);
			void place_camera();
			void use_editor_camera();


		private:
			std::string _path;

			FrameBuffer* _output_buffer;

			float _mouse_x = 0;
			float _mouse_y = 0;

			int _running_counter = 0;
			bool _can_render = true;

			
	};



}
