#include "PhysicsEngine.h"


#include <libs/PhysX/PhysX.h>

#include "Logging.h"
#include <format>

#include "physics/utils.h"
#include "physics/types.h"

namespace ph{
	// namespace physics{
		
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

			// physx::PxMaterial* material = nullptr;
			// physx::PxRigidDynamic* actor = nullptr;
			// physx::PxShape* shape = nullptr;
			PhysicsRigidDynamic box{};

			PhysicsRigidStatic ground{};
		};



		//////////////////////////////////////////////////////////////////////
		// physics

		// TODO: some funcs can return nullptr
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
			//     // fatalError("PxInitExtensions failed!");
			// }


			this->backend->dispatcher = physx::PxDefaultCpuDispatcherCreate(2);

			auto scene_description = physx::PxSceneDesc(this->backend->physics->getTolerancesScale());
			scene_description.gravity        = physx::PxVec3(0.0f, -9.81f, 0.0f);
			scene_description.cpuDispatcher	 = this->backend->dispatcher;
			scene_description.filterShader	 = physx::PxDefaultSimulationFilterShader;

			this->backend->scene = this->backend->physics->createScene(scene_description);



			this->backend->box.create_cube(
				this->backend->physics, this->backend->scene,
				0.5f, 0.5f, 0.75f,
				0.5f, 0.5f, 0.5f,
				physx::PxTransform{ physx::PxVec3{0.0f, 10.0f, 0.0f} }
			);

			this->backend->box.set_density(1.5f);



			this->backend->ground.create_cube(
				this->backend->physics, this->backend->scene,
				0.5f, 0.5f, 0.9f,
				20.0f, 0.5f, 20.0f,
				physx::PxTransform{ physx::PxVec3{0.0f, -0.5f, 0.0f} }
			);



			PH_INFO("Initialized: Physics (NVIDIA PhysX 5.2)");
			return true;
		};



		auto PhysicsEngine::shutdown() noexcept -> void {

			this->backend->ground.destroy(this->backend->scene);

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

		




		auto PhysicsEngine::get_body() noexcept -> glm::mat4 {
			// const physx::PxMat44 shapePose(physx::PxShapeExt::getGlobalPose(*this->backend->shape, *this->backend->actor));

			return PhysX_to_glm(this->backend->box.get_transform());
		};


		auto PhysicsEngine::simulate(float dt) noexcept -> void {
			this->backend->scene->simulate(dt);
			this->backend->scene->fetchResults(true);
		};


	// };
};