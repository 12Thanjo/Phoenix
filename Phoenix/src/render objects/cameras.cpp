#include "ph_pch.h"
#include "cameras.h"

namespace Phoenix{

	//////////////////////////////////////////////////////////////////////
	// PerspectiveCamera

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect_ratio, float near, float far)
		:  _fov(fov),  _aspect_ratio(aspect_ratio),  _near(near),  _far(far) {

		calculate_view();
		calculate_projection();

	}



	void PerspectiveCamera::moveForward(float amount){
		_position += amount * _front;
		calculate_view();
	}

	void PerspectiveCamera::moveBackward(float amount){
		_position -= amount * _front;
		calculate_view();
	}

	void PerspectiveCamera::moveLeft(float amount){
		_position -= glm::normalize(glm::cross(_front, _up)) * amount;
		calculate_view();
	}

	void PerspectiveCamera::moveRight(float amount){
		_position += glm::normalize(glm::cross(_front, _up)) * amount;
		calculate_view();
	}

	void PerspectiveCamera::moveUp(float amount){
		_position += _up * amount;
		calculate_view();
	}

	void PerspectiveCamera::moveDown(float amount){
		_position -= _up * amount;
		calculate_view();
	}



	void PerspectiveCamera::setPosition(glm::vec3 position){
		_position = position;
		calculate_view();
	}


	void PerspectiveCamera::setRotation(glm::vec3 rotation){
		// x: roll
		// y: yaw
		// z: pitch

		rotation.x = fmod(rotation.x, 6.283185307179586f);
		rotation.y = fmod(rotation.y, 6.283185307179586f);
		rotation.z = fmod(rotation.z, 6.283185307179586f);

		_rotation = rotation;

		
		_front = glm::normalize( glm::vec3(
			cos(_rotation.y) * cos(_rotation.z),
			sin(_rotation.z),
			sin(_rotation.y) * cos(_rotation.z)
		));

		calculate_view();
	}





	void PerspectiveCamera::setProjection(float fov, float aspect_ratio, float near, float far){
		_fov = fov;
		_aspect_ratio = aspect_ratio;
		_near = near;
		_far = far;

		// PH_LOG("set projection");

		// calculate_view();
		calculate_projection();
	}



	void PerspectiveCamera::calculate_view(){
		_view = glm::rotate(glm::mat4(1.0f), _rotation.x, {0, 0, 1}) * glm::lookAt(_position, _position + _front, _up);
	}

	void PerspectiveCamera::calculate_projection(){
		_projection = glm::perspective(_fov, _aspect_ratio, _near, _far);
	}



	//////////////////////////////////////////////////////////////////////
	// OrbitalCamera

	OrbitalCamera::OrbitalCamera(float fov, float aspect_ratio, float near, float far)
		:  _fov(fov),  _aspect_ratio(aspect_ratio),  _near(near),  _far(far) {

		calculate_view();
		calculate_projection();

	}




	void OrbitalCamera::setPosition(glm::vec3 position){
		_position = position;
		calculate_view();
	}



	void OrbitalCamera::setRotation(glm::vec3 rotation){
		// x: roll
		// y: yaw
		// z: pitch

		rotation.x = fmod(rotation.x, 6.283185307179586f);
		rotation.y = fmod(rotation.y, 6.283185307179586f);
		rotation.z = fmod(rotation.z, 6.283185307179586f);

		_rotation = rotation;
		
		_front = glm::normalize( glm::vec3(
			cos(_rotation.y) * cos(_rotation.z),
			sin(_rotation.z),
			sin(_rotation.y) * cos(_rotation.z)
		));

		calculate_view();
	}


	void OrbitalCamera::setFocalPoint(glm::vec3 focal_point){
		_focal_point = focal_point;
		place();
	}




	void OrbitalCamera::setProjection(float fov, float aspect_ratio, float near, float far){
		_fov = fov;
		_aspect_ratio = aspect_ratio;
		_near = near;
		_far = far;


		calculate_projection();
	}



	void OrbitalCamera::setCoordinates(float rho, float theta, float phi){
		_rho = rho;
		_theta = theta;
		_phi = phi;

		place();
	}


	void OrbitalCamera::calculate_view(){
		_view = glm::rotate(glm::mat4(1.0f), _rotation.x, {0, 0, 1}) * glm::lookAt(_position, _position + _front, _up);
	}

	void OrbitalCamera::calculate_projection(){
		_projection = glm::perspective(_fov, _aspect_ratio, _near, _far);
	}


	void OrbitalCamera::place(){
		if(_phi < 0.0f){
			_phi = 0.0f;
		}else if(_phi > 3.14f - 0.01f){
			_phi = 3.14f - 0.01f;
		};


		_position.x = (_rho * cos(_theta) * sin(_phi)) + _focal_point.x;
		_position.z = (_rho * sin(_theta) * sin(_phi)) + _focal_point.z;
		_position.y = (_rho * cos(_phi)) + _focal_point.y;

		_rotation.z = _phi - (3.14f*0.5f);
		_rotation.y = _theta - 3.14f;


		calculate_view();
		setRotation(_rotation);
	}

}
