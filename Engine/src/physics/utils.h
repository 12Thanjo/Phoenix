#pragma once


#include <libs/Evo/Evo.h>

#include <libs/glm/glm.h>
#include <libs/PhysX/PhysX.h>


namespace ph{

	//////////////////////////////////////////////////////////////////////
	// vec3

	EVO_NODISCARD inline auto PhysX_to_glm(const physx::PxVec3& physx_vec) noexcept -> glm::vec3 {
		return glm::vec3{physx_vec.x, physx_vec.y, physx_vec.z};
	};


	EVO_NODISCARD inline auto glm_to_PhysX(const glm::vec3& glm_vec) noexcept -> physx::PxVec3 {
		return physx::PxVec3{glm_vec.x, glm_vec.y, glm_vec.z};
	};



	//////////////////////////////////////////////////////////////////////
	// mat4
		
	EVO_NODISCARD inline auto PhysX_to_glm(const physx::PxMat44& physx_mat) noexcept -> glm::mat4 {
		auto glm_mat = glm::mat4{};

		for(int row = 0; row < 4; row+=1){
			for(int collumn = 0; collumn < 4; collumn+=1){
				glm_mat[row][collumn] = physx_mat[row][collumn];
			}
		}

		return glm_mat;
	};

	EVO_NODISCARD inline auto glm_to_PhysX(const glm::mat4& glm_mat) noexcept -> physx::PxMat44 {
		auto physx_mat = physx::PxMat44{};

		for(int row = 0; row < 4; row+=1){
			for(int collumn = 0; collumn < 4; collumn+=1){
				physx_mat[row][collumn] = glm_mat[row][collumn];
			}
		}

		return physx_mat;
	};

};