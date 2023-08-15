#pragma once

#include <libs/Evo/Evo.h>

#include <libs/glm/glm.h>

namespace ph{
		
	class PhysicsEngine{
		public:
			PhysicsEngine() = default;
			~PhysicsEngine() = default;

			EVO_NODISCARD auto init() noexcept -> bool;
			auto shutdown() noexcept -> void;

			auto get_body() noexcept -> glm::mat4;

			auto simulate(float dt) noexcept -> void;
	
		private:
			struct PhysicsBackend* backend;
	};
	
};