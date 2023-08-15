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

				return this->interface->create_static_cube(
					(alias::Vec3)&position, (alias::Vec3)&scale, static_friction, dynamic_friction, restitution
				);

			};



			///////////////////////////////////
			// dynamic colliders


			PH_NODISCARD inline auto createDynamicCube(
				glm::vec3 position, glm::vec3 scale, float static_friction, float dynamic_friction, float restitution
			) noexcept -> DynamicCollider {

				return this->interface->create_dynamic_cube(
					(alias::Vec3)&position, (alias::Vec3)&scale, static_friction, dynamic_friction, restitution
				);

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


			
	
		private:
			EngineInterface* interface;
	};

};
