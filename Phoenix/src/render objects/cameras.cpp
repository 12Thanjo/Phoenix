#include "ph_pch.h"
#include "cameras.h"

namespace Phoenix{

	Camera::Camera(float fov, float aspect_ratio, float near, float far)
		:  _fov(fov),  _aspect_ratio(aspect_ratio),  _near(near),  _far(far) {

		calculate_view();
		calculate_projection();
	}



	void Camera::moveForward(float amount){
		_position += amount * _front;
		calculate_view();
	}

	void Camera::moveBackward(float amount){
		_position -= amount * _front;
		calculate_view();
	}

	void Camera::moveLeft(float amount){
		_position -= glm::normalize(glm::cross(_front, _up)) * amount;
		calculate_view();
	}

	void Camera::moveRight(float amount){
		_position += glm::normalize(glm::cross(_front, _up)) * amount;
		calculate_view();
	}

	void Camera::moveUp(float amount){
		_position += _up * amount;
		calculate_view();
	}

	void Camera::moveDown(float amount){
		_position -= _up * amount;
		calculate_view();
	}



	void Camera::setPosition(glm::vec3 position){
		_position = position;
		calculate_view();
	}


	void Camera::setRotation(glm::vec3 rotation){
		// x: roll
		// y: yaw
		// z: pitch

		_rotation = rotation;

		
		_front = glm::normalize( glm::vec3(
			cos(_rotation.y) * cos(_rotation.z),
			sin(_rotation.z),
			sin(_rotation.y) * cos(_rotation.z)
		));

		calculate_view();
	}





	void Camera::setProjection(float fov, float aspect_ratio, float near, float far){
		_fov = fov;
		_aspect_ratio = aspect_ratio;
		_near = near;
		_far = far;

		calculate_view();
		calculate_projection();
	}



	glm::mat4 Camera::getViewProjection(){
		return _projection * _view;
	}




	void Camera::calculate_view(){
		_view = glm::rotate(glm::mat4(1.0f), _rotation.x, {0, 0, 1}) * glm::lookAt(_position, _position + _front, _up);
	}

	void Camera::calculate_projection(){
		_projection = glm::perspective(_fov, _aspect_ratio, _near, _far);
	}

}
