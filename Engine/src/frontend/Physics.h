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


			///////////////////////////////////
			// static colliders

			PH_NODISCARD inline auto createStaticCube(
				glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
			) noexcept -> StaticCollider {

				return this->interface->create_static_cube(position, scale, static_friction, dynamic_friction, restitution);
			};



			///////////////////////////////////
			// dynamic colliders


			PH_NODISCARD inline auto createDynamicCube(
				glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
			) noexcept -> DynamicCollider {

				return this->interface->create_dynamic_cube(position, scale, static_friction, dynamic_friction, restitution);
			};


			inline auto setDynamicColliderDensity(DynamicCollider collider, float density) noexcept -> void {
				this->interface->set_dynamic_collider_density(collider, density);
			};

			inline auto setDynamicColliderMass(DynamicCollider collider, float mass) noexcept -> void {
				this->interface->set_dynamic_collider_mass(collider, mass);
			};


			PH_NODISCARD inline auto getDynamicColliderTransform(DynamicCollider collider) noexcept -> glm::mat4 {
				return this->interface->get_dynamic_collider_transform(collider);
			};



			///////////////////////////////////
			// character controller


			PH_NODISCARD inline auto createCharacterController(glm::vec3 position, float height, float radius) noexcept -> CharacterController {
				return this->interface->create_character_controller(position, height, radius);
			};


			PH_NODISCARD inline auto getCharacterControllerPosition(CharacterController controller) noexcept -> glm::vec3 {
				return this->interface->get_character_controller_position(controller);
			};


			PH_NODISCARD inline auto characterControllerMove(CharacterController controller, glm::vec3 direction, float dt) noexcept -> void {
				return this->interface->character_controller_move(controller, direction, dt);
			};

			
	
		private:
			EngineInterface* interface;
	};

};
