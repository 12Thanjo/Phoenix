#pragma once

#include <Phoenix.h>

#include "Project.h"

namespace Phoenix{

	
	class Game : public Engine{
		public:
			Game(std::string file);
			~Game();
			
			void create() override;
			void render() override;

		public:
			Project project;
			std::string path;
			std::string project_path;


		private:
			FrameBuffer* _output_buffer;

			float _mouse_x = 0;
			float _mouse_y = 0;
			
	};



}
