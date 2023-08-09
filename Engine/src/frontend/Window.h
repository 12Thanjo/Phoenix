#pragma once

#include "../../include/Phoenix/defines.h"
#include "./EngineInterface.h"


namespace ph{


	
	class Window{
		public:
			Window(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~Window() = default;



			auto getSize() noexcept -> std::pair<int, int> {
				auto output = std::pair<int, int>{};
				this->interface->get_window_size(&output.first, &output.second);
				return output;
			};


			auto captureMouse() noexcept -> void { this->interface->capture_mouse(); };
			auto releaseMouse() noexcept -> void { this->interface->release_mouse(); };


	
		private:
			EngineInterface* interface;
	};

};
