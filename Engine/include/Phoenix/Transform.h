#pragma once


#include <libs/glm/glm.h>

namespace ph{


	struct Transform{
		glm::vec3 position{0.0, 0.0, 0.0};
		glm::vec3 rotation{0.0, 0.0, 0.0};
		glm::vec3 scale{1.0f, 1.0f, 1.0f};

		PH_NODISCARD inline auto calculate() const noexcept -> glm::mat4 {
			glm::mat4 transform = glm::mat4{1.0f};

			transform = glm::translate(transform, this->position);

			// transform = glm::inverse(transform);

			transform = glm::rotate(transform, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
			transform = glm::rotate(transform, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

			transform = glm::scale(transform, this->scale);
			// transform *= glm::inverse( glm::translate(glm::mat4(1.0f), this->position) );

			return transform;
		};
	};




	struct Transform2D{
		float x = 0.0f;
		float y = 0.0f;
		float rotation = 0.0f;
		float width = 1.0f;
		float height = 1.0f;


		PH_NODISCARD inline auto calculate() const noexcept -> glm::mat4 {
			glm::mat4 transform = glm::translate(glm::mat4{1.0f}, {this->x, this->y, 0.0f});

			transform = glm::rotate(transform, this->rotation, {0.0f, 0.0f, 1.0f});

			transform = glm::scale(transform, {this->width, this->height, 1.0f});

			return transform;
		};
	};




	struct Camera{
		glm::vec3 position{0.0, 0.0, 0.0};
		glm::vec3 rotation{0.0, 0.0, 0.0};

		PH_NODISCARD inline auto calculate() const noexcept -> glm::mat4 {
			glm::mat4 transform = glm::mat4{1.0f};

			transform = glm::rotate(transform, this->rotation.x, glm::vec3(-1.0f, 0.0f, 0.0f));
			transform = glm::rotate(transform, this->rotation.y, glm::vec3(0.0f, -1.0f, 0.0f));
			transform = glm::rotate(transform, this->rotation.z, glm::vec3(0.0f, 0.0f, -1.0f));

			transform *= glm::inverse( glm::translate(glm::mat4(1.0f), this->position) );

			return transform;
		};
	};


	auto getTransformForward(const glm::mat4& transform) noexcept -> glm::vec3 {
		return glm::normalize( glm::vec3{ -transform[2][0], -transform[2][1], -transform[2][2] } );
	};

	auto getTransformBackward(const glm::mat4& transform) noexcept -> glm::vec3 {
		return glm::normalize( glm::vec3{ transform[2][0], transform[2][1], transform[2][2] } );
	};


	auto getTransformUp(const glm::mat4& transform) noexcept -> glm::vec3 {
		return glm::normalize( glm::vec3{ -transform[1][0], -transform[1][1], -transform[1][2] } );
	};

	auto getTransformDown(const glm::mat4& transform) noexcept -> glm::vec3 {
		return glm::normalize( glm::vec3{ transform[1][0], transform[1][1], transform[1][2] } );
	};


	auto getTransformLeft(const glm::mat4& transform) noexcept -> glm::vec3 {
		return glm::normalize( glm::vec3{ -transform[0][0], -transform[0][1], -transform[0][2] } );
	};

	auto getTransformRight(const glm::mat4& transform) noexcept -> glm::vec3 {
		return glm::normalize( glm::vec3{ transform[0][0], transform[0][1], transform[0][2] } );
	};
	

};