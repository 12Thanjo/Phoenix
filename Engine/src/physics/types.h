#pragma once


#include <libs/Evo/Evo.h>

#include <libs/PhysX/PhysX.h>
#include <libs/glm/glm.h>


namespace ph{


	//////////////////////////////////////////////////////////////////////
	// static


	class PhysicsRigidStatic{
		public:
			PhysicsRigidStatic() = default;
			~PhysicsRigidStatic() = default;

			auto create_cube(
				physx::PxPhysics* physics, physx::PxScene* scene,
				float static_friction, float dynamic_friction, float restitution,
				float width, float height, float depth,
				physx::PxMat44 transform
			) noexcept -> void;

			auto destroy(physx::PxScene* scene) noexcept -> void;

	
		private:
			physx::PxMaterial* material = nullptr;
			physx::PxShape* shape = nullptr;
			physx::PxRigidStatic* actor = nullptr;
	};




	//////////////////////////////////////////////////////////////////////
	// dynamic

	class PhysicsRigidDynamic{
		public:
			PhysicsRigidDynamic() = default;
			~PhysicsRigidDynamic() = default;

			auto create_cube(
				physx::PxPhysics* physics, physx::PxScene* scene,
				float static_friction, float dynamic_friction, float restitution,
				float width, float height, float depth,
				physx::PxMat44 transform
			) noexcept -> void;

			auto destroy(physx::PxScene* scene) noexcept -> void;


			auto set_mass(float mass) noexcept -> void;
			auto set_density(float density) noexcept -> void;


			auto get_transform() const noexcept -> physx::PxMat44;

	
		private:
			physx::PxMaterial* material = nullptr;
			physx::PxShape* shape = nullptr;
			physx::PxRigidDynamic* actor = nullptr;
	};



	//////////////////////////////////////////////////////////////////////
	// character controller


	class PhysicsCharacterController{
		public:
			PhysicsCharacterController() = default;
			~PhysicsCharacterController() = default;

			auto create(
				physx::PxPhysics* physics, physx::PxControllerManager* controller_manager, glm::vec3 position, float height, float radius
			) noexcept -> void;

			auto destroy() noexcept -> void;


			auto move(glm::vec3 direction, float dt, float min_distance = 0.0f) noexcept -> void;

			EVO_NODISCARD auto get_position() const noexcept -> glm::vec3;

			EVO_NODISCARD inline auto is_colliding_below() const noexcept -> bool { return this->collision_below; };
			EVO_NODISCARD inline auto is_colliding_above() const noexcept -> bool { return this->collision_above; };
			EVO_NODISCARD inline auto is_colliding_side() const noexcept -> bool { return this->collision_side; };
	
		private:
			physx::PxController* controller = nullptr;

			bool collision_below = false;
			bool collision_above = false;
			bool collision_side = false;
	};


		
};