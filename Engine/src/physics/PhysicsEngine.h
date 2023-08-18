#pragma once

#include <libs/Evo/Evo.h>

#include <libs/glm/glm.h>

#include "frontend/common.h"

namespace ph{

	struct PhysicsMaterial{
		float static_friction;
		float dynamic_friction;
		float restitution;
	};


		
	class PhysicsEngine{
		public:
			PhysicsEngine() = default;
			~PhysicsEngine() = default;

			EVO_NODISCARD auto init() noexcept -> bool;
			auto shutdown() noexcept -> void;

			auto simulate(float dt) noexcept -> void;



			///////////////////////////////////
			// colliders

			// static
			PH_NODISCARD auto create_static_cube(glm::vec3 position, glm::vec3 scale, PhysicsMaterial material) noexcept -> StaticCollider;


			// dynamic
			PH_NODISCARD auto create_dynamic_cube(glm::vec3 position, glm::vec3 scale, PhysicsMaterial material) noexcept -> DynamicCollider;

			auto set_dynamic_collider_density(DynamicCollider collider, float density) noexcept -> void;
			auto set_dynamic_collider_mass(DynamicCollider collider, float mass) noexcept -> void;

			PH_NODISCARD auto get_dynamic_collider_transform(DynamicCollider collider) const noexcept -> glm::mat4;


			// character controller
			PH_NODISCARD auto create_character_controller(glm::vec3 position, float height, float radius) noexcept -> CharacterController;

			PH_NODISCARD auto get_character_controller_position(CharacterController controller) const noexcept -> glm::vec3;

			auto character_controller_move(CharacterController controller, glm::vec3 direction, float dt) noexcept -> void;

			PH_NODISCARD auto get_character_controller_grounded(CharacterController controller) const noexcept -> bool;


	
		private:
			struct PhysicsBackend* backend;
	};
	
};