#pragma once


#include <libs/Evo/Evo.h>

#include <libs/PhysX/PhysX.h>


namespace ph{


	//////////////////////////////////////////////////////////////////////
	// static


	class PhysicsRigidStatic{
		public:
			PhysicsRigidStatic() = default;
			~PhysicsRigidStatic() = default;

			EVO_NODISCARD auto create_cube(
				physx::PxPhysics* physics, physx::PxScene* scene,
				float static_friction, float dynamic_friction, float restitution,
				float width, float height, float depth,
				physx::PxMat44 transform
			) noexcept -> bool;

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

			EVO_NODISCARD auto create_cube(
				physx::PxPhysics* physics, physx::PxScene* scene,
				float static_friction, float dynamic_friction, float restitution,
				float width, float height, float depth,
				physx::PxMat44 transform
			) noexcept -> bool;

			auto destroy(physx::PxScene* scene) noexcept -> void;


			auto set_mass(float mass) noexcept -> void;
			auto set_density(float density) noexcept -> void;


			auto get_transform() const noexcept -> physx::PxMat44;

	
		private:
			physx::PxMaterial* material = nullptr;
			physx::PxShape* shape = nullptr;
			physx::PxRigidDynamic* actor = nullptr;
	};


		
};