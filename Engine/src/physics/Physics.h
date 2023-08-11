#pragma once

#include <libs/Evo/Evo.h>



namespace ph{
	
	class Physics{
		public:
			Physics() = default;
			~Physics() = default;

			EVO_NODISCARD auto init() noexcept -> bool;
			auto shutdown() noexcept -> void;
	
		private:
			struct PhysicsBackend* backend;
	};

};