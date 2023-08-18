#include "types.h"


#include "Logging.h"
#include <format>

#include "physics/utils.h"


namespace ph{


	//////////////////////////////////////////////////////////////////////
	// static

	auto PhysicsRigidStatic::create_cube(
		physx::PxPhysics* physics, physx::PxScene* scene,
		float static_friction, float dynamic_friction, float restitution,
		float width, float height, float depth,
		physx::PxMat44 transform
	) noexcept -> void {

		this->material = physics->createMaterial(static_friction, dynamic_friction, restitution);

		this->shape = physics->createShape(physx::PxBoxGeometry{width, height, depth}, *this->material, true);

		this->actor = physics->createRigidStatic(physx::PxTransform{transform});
		this->actor->attachShape(*this->shape);

		scene->addActor(*this->actor);


		PH_TRACE("Created: PhysX rigid static (cube)");
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
	) noexcept -> void {

		this->material = physics->createMaterial(static_friction, dynamic_friction, restitution);

		this->shape = physics->createShape(physx::PxBoxGeometry{width, height, depth}, *this->material, true);

		this->actor = physics->createRigidDynamic(physx::PxTransform{transform});
		this->actor->attachShape(*this->shape);

		scene->addActor(*this->actor);

		PH_TRACE("Created: PhysX rigid dynamic (cube)");
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




	//////////////////////////////////////////////////////////////////////
	// character controller


	auto PhysicsCharacterController::create(
		physx::PxPhysics* physics, physx::PxControllerManager* controller_manager, glm::vec3 position, float height, float radius
	) noexcept -> void {

		auto description = physx::PxCapsuleControllerDesc{};
		description.position   = glm_to_PhysX_extended(position);
		description.height     = height / 2.0f;
		description.radius     = radius;
		description.stepOffset = 0.1f;
		description.material   = physics->createMaterial(0.5f, 0.5f, 0.6f);


		this->controller = controller_manager->createController(description);

		PH_TRACE("Created: Physics character controller");
	};


	auto PhysicsCharacterController::destroy() noexcept -> void {

		this->controller->release();
		
		PH_TRACE("Destroyed: Physics character controller");
	};


	auto PhysicsCharacterController::move(glm::vec3 direction, float dt, float min_distance) noexcept -> void {
		const physx::PxVec3 displacement = glm_to_PhysX(direction);
		const physx::PxControllerFilters filters = {};

		const physx::PxControllerCollisionFlags collision_flags = this->controller->move(displacement, min_distance, dt, filters, nullptr);

		this->collision_below = collision_flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_DOWN);
		this->collision_above = collision_flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_UP);
		this->collision_side = collision_flags.isSet(physx::PxControllerCollisionFlag::eCOLLISION_SIDES);
	};


	auto PhysicsCharacterController::get_position() const noexcept -> glm::vec3 {
		return extended_PhysX_to_glm(this->controller->getPosition());
	};

		
};