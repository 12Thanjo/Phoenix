#include "PhysicsEngine.h"


#include <libs/PhysX/PhysX.h>

#include "Logging.h"
#include <format>

#include "physics/utils.h"
#include "physics/types.h"

namespace ph{
		
	//////////////////////////////////////////////////////////////////////
	// helper types


	// TODO: rewrite PhysX to remove debug info from binary in release mode
	// TODO: rewrite PhysX to remove debug info from binary in release mode
	// TODO: rewrite PhysX to remove debug info from binary in release mode
	// TODO: rewrite PhysX to remove debug info from binary in release mode

	class PhysicsErrorCallback : public physx::PxErrorCallback {
		public:
		    virtual void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line){
		    	auto error_code = evo::StaticString<19>();

		    	switch(code){
		    		break;case physx::PxErrorCode::eNO_ERROR:          error_code = "No Error";
		    		break;case physx::PxErrorCode::eINVALID_PARAMETER: error_code = "Invalid Parameter";
		    		break;case physx::PxErrorCode::eINVALID_OPERATION: error_code = "Invalid Operation";
		    		break;case physx::PxErrorCode::eOUT_OF_MEMORY:     error_code = "Out of Memory";
		    		break;case physx::PxErrorCode::eDEBUG_INFO:        error_code = "Info";
		    		break;case physx::PxErrorCode::eDEBUG_WARNING:     error_code = "Warning";
		    		break;case physx::PxErrorCode::ePERF_WARNING:      error_code = "Performance Warning";
		    		break;case physx::PxErrorCode::eABORT:             error_code = "Abort";
		    		break;case physx::PxErrorCode::eINTERNAL_ERROR:    error_code = "Internal Error";
		    		break;case physx::PxErrorCode::eMASK_ALL:          error_code = "Unknown Error";
		    	};


		    	if(code == physx::PxErrorCode::eDEBUG_INFO){
		        	PH_INFO(std::format("PhysX info: ({}) '{}'\n\tfile: {}\n\tline: {}", error_code, message, file, line));
		    		
		    	}else if(code == physx::PxErrorCode::eDEBUG_WARNING){
		        	PH_WARNING(std::format("PhysX warning: ({}) '{}'\n\tfile: {}\n\tline: {}", error_code, message, file, line));

		    	}else{
		        	PH_ERROR(std::format("PhysX error: ({}) '{}'\n\tfile: {}\n\tline: {}", error_code, message, file, line));
		    	}

		    }
	};


	struct PhysicsBackend{
		physx::PxDefaultAllocator allocator{};
		PhysicsErrorCallback error_callback{};

		physx::PxFoundation* foundation = nullptr;
		physx::PxPhysics* physics = nullptr;
		physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
		physx::PxScene* scene = nullptr;
		physx::PxControllerManager* controller_manager = nullptr;

		std::vector<PhysicsRigidStatic> static_colliders{};
		std::vector<PhysicsRigidDynamic> dynamic_colliders{};
		std::vector<PhysicsCharacterController> character_controllers{};
	};



	//////////////////////////////////////////////////////////////////////
	// physics

	auto PhysicsEngine::init() noexcept -> bool {
		this->backend = new PhysicsBackend();

		this->backend->foundation = PxCreateFoundation(PX_PHYSICS_VERSION, this->backend->allocator, this->backend->error_callback);
		if(this->backend->foundation == nullptr){
			PH_FATAL("Failed to initialize PhysX foundation");
			return false;
		}


		constexpr bool record_memory_allocations = false;
		constexpr physx::PxPvd* pvd = nullptr;
		this->backend->physics = PxCreatePhysics(
			PX_PHYSICS_VERSION, *this->backend->foundation, physx::PxTolerancesScale(), record_memory_allocations, pvd
		);

		if(this->backend->physics == nullptr){
			PH_FATAL("Failed to initialize PhysX physics");
			return false;
		}



		// if(!PxInitExtensions(*this->backend->physics, nullptr)){
		//     // PH_FATAL("PxInitExtensions failed!");
		// }


		this->backend->dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

		auto scene_description = physx::PxSceneDesc(this->backend->physics->getTolerancesScale());
		scene_description.gravity        = physx::PxVec3(0.0f, -9.81f, 0.0f);
		scene_description.cpuDispatcher	 = this->backend->dispatcher;
		scene_description.filterShader	 = physx::PxDefaultSimulationFilterShader;

		this->backend->scene = this->backend->physics->createScene(scene_description);


		this->backend->controller_manager = PxCreateControllerManager(*this->backend->scene);


		PH_INFO("Initialized: Physics (NVIDIA PhysX 5.2)");
		return true;
	};



	auto PhysicsEngine::shutdown() noexcept -> void {


		for(auto& static_collider : this->backend->static_colliders){
			static_collider.destroy(this->backend->scene);
		}

		for(auto& dynamic_collider : this->backend->dynamic_colliders){
			dynamic_collider.destroy(this->backend->scene);
		}

		for(auto& character_controller : this->backend->character_controllers){
			character_controller.destroy();
		}



		if(this->backend->controller_manager != nullptr){
			this->backend->controller_manager->release();
		}		

		if(this->backend->scene != nullptr){
			this->backend->scene->release();
		}

		if(this->backend->dispatcher != nullptr){
			this->backend->dispatcher->release();
		}

		if(this->backend->physics != nullptr){
			this->backend->physics->release();
		}

		if(this->backend->foundation != nullptr){
			this->backend->foundation->release();
		}

		delete this->backend;

		PH_INFO("Shutdown: Physics (NVIDIA PhysX 5.2)");
	};

	



	auto PhysicsEngine::simulate(float dt) noexcept -> void {
		this->backend->scene->simulate(dt);
		this->backend->scene->fetchResults(true);
	};





	//////////////////////////////////////////////////////////////////////
	// colliders

	///////////////////////////////////
	// static

	auto PhysicsEngine::create_static_cube(glm::vec3 position, glm::vec3 scale, PhysicsMaterial material) noexcept -> StaticCollider {

		auto& new_collider = this->backend->static_colliders.emplace_back();

		new_collider.create_cube(
			this->backend->physics, this->backend->scene,
			material.static_friction, material.dynamic_friction, material.restitution,
			scale.x, scale.y, scale.z,
			physx::PxTransform{ glm_to_PhysX(position) }
		);


		return StaticCollider{ uint32_t(this->backend->static_colliders.size() - 1) };
	};



	///////////////////////////////////
	// dynamic


	auto PhysicsEngine::create_dynamic_cube(glm::vec3 position, glm::vec3 scale, PhysicsMaterial material) noexcept -> DynamicCollider {

		auto& new_collider = this->backend->dynamic_colliders.emplace_back();

		new_collider.create_cube(
			this->backend->physics, this->backend->scene,
			material.static_friction, material.dynamic_friction, material.restitution,
			scale.x, scale.y, scale.z,
			physx::PxTransform{ glm_to_PhysX(position) }
		);


		return DynamicCollider{ uint32_t(this->backend->dynamic_colliders.size() - 1) };
	};



	auto PhysicsEngine::set_dynamic_collider_density(DynamicCollider collider, float density) noexcept -> void {
		this->backend->dynamic_colliders[collider.id].set_density(density);
	};

	auto PhysicsEngine::set_dynamic_collider_mass(DynamicCollider collider, float mass) noexcept -> void {
		this->backend->dynamic_colliders[collider.id].set_mass(mass);
	};


	auto PhysicsEngine::get_dynamic_collider_transform(DynamicCollider collider) const noexcept -> glm::mat4 {
		return PhysX_to_glm(this->backend->dynamic_colliders[collider.id].get_transform());
	};


	///////////////////////////////////
	// character controller


	auto PhysicsEngine::create_character_controller(glm::vec3 position, float height, float radius) noexcept -> CharacterController {

		auto& new_controller = this->backend->character_controllers.emplace_back();

		new_controller.create(this->backend->physics, this->backend->controller_manager, position, height, radius);

		return CharacterController{ uint32_t(this->backend->character_controllers.size() - 1) };
	};


	auto PhysicsEngine::get_character_controller_position(CharacterController controller) const noexcept -> glm::vec3 {
		return this->backend->character_controllers[controller.id].get_position();
	};


	auto PhysicsEngine::character_controller_move(CharacterController controller, glm::vec3 direction, float dt) noexcept -> void {
		return this->backend->character_controllers[controller.id].move(direction, dt, 0.0f);
	};



	auto PhysicsEngine::get_character_controller_grounded(CharacterController controller) const noexcept -> bool {
		return this->backend->character_controllers[controller.id].is_colliding_below();
	};

		


};