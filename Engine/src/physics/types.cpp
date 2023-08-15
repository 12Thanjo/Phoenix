#include "types.h"



#include "Logging.h"
#include <format>


namespace ph{


	//////////////////////////////////////////////////////////////////////
	// static

	auto PhysicsRigidStatic::create_cube(
		physx::PxPhysics* physics, physx::PxScene* scene,
		float static_friction, float dynamic_friction, float restitution,
		float width, float height, float depth,
		physx::PxMat44 transform
	) noexcept -> bool {

		this->material = physics->createMaterial(static_friction, dynamic_friction, restitution);

		this->shape = physics->createShape(physx::PxBoxGeometry{width, height, depth}, *this->material, true);

		this->actor = physics->createRigidStatic(physx::PxTransform{transform});
		this->actor->attachShape(*this->shape);

		scene->addActor(*this->actor);


		PH_TRACE("Created: PhysX rigid static (cube)");
		return true;
	};


	auto PhysicsRigidStatic::destroy(physx::PxScene* scene) noexcept -> void {

		scene->removeActor(*this->actor);

		this->actor->release();
		this->shape->release();
		this->material->release();

		PH_TRACE("Destroyed: PhysX rigid static");
	};




	//////////////////////////////////////////////////////////////////////
	// dynamic


	auto PhysicsRigidDynamic::create_cube(
		physx::PxPhysics* physics, physx::PxScene* scene,
		float static_friction, float dynamic_friction, float restitution,
		float width, float height, float depth,
		physx::PxMat44 transform
	) noexcept -> bool {

		this->material = physics->createMaterial(static_friction, dynamic_friction, restitution);

		this->shape = physics->createShape(physx::PxBoxGeometry{width, height, depth}, *this->material, true);

		this->actor = physics->createRigidDynamic(physx::PxTransform{transform});
		this->actor->attachShape(*this->shape);

		scene->addActor(*this->actor);

		PH_TRACE("Created: PhysX rigid dynamic (cube)");
		return true;
	};



	auto PhysicsRigidDynamic::destroy(physx::PxScene* scene) noexcept -> void {

		scene->removeActor(*this->actor);

		this->actor->release();
		this->shape->release();
		this->material->release();

		PH_TRACE("Destroyed: PhysX rigid dynamic");
	};



	auto PhysicsRigidDynamic::set_mass(float mass) noexcept -> void {
		this->actor->setMass(mass);
	};


	auto PhysicsRigidDynamic::set_density(float density) noexcept -> void {
		physx::PxRigidBodyExt::updateMassAndInertia(*this->actor, density);
	};



	auto PhysicsRigidDynamic::get_transform() const noexcept -> physx::PxMat44 {
		const physx::PxTransform pose = physx::PxShapeExt::getGlobalPose(*this->shape, *this->actor);
		return physx::PxMat44{pose};
	};

		
};