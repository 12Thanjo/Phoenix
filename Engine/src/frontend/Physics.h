#pragma once

#include "../../include/Phoenix/defines.h"

#include "./EngineInterface.h"

#include "./common.h"

#include <libs/glm/glm.h>


namespace ph{
	
	class Physics{
		public:
			Physics(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~Physics() = default;


			
	
		private:
			EngineInterface* interface;
	};

};
