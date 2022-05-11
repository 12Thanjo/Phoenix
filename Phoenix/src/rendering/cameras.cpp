#include "ph_pch.h"
#include "cameras.h"


namespace Phoenix{

	Camera::Camera(glm::mat4 ortho)
		: projection_matrix( ortho ) {


		view_projection_matrix = projection_matrix * view_matrix;
	}

	



	OrthographicCamera::OrthographicCamera(float width, float height)
		: Camera( glm::ortho(width/-2, width/2, height/-2, height/2, -1.0f, 1.0f) ){

		view_projection_matrix = projection_matrix * view_matrix; //must multiply in this order
	}


	void OrthographicCamera::calculate_view_matrix(){
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f));

		// transform = glm::rotate(transform, rotation.x, glm::vec3(1, 0, 0));
		// transform = glm::rotate(transform, rotation.y, glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, rotation, glm::vec3(0, 0, 1));

		view_matrix = glm::inverse(transform);
		view_projection_matrix = projection_matrix * view_matrix; //must multiply in this order

	}


	void OrthographicCamera::set_projection(float width, float height){
		projection_matrix = glm::ortho(width/-2, width/2, height/-2, height/2, -1.0f, 1.0f);
		view_projection_matrix = projection_matrix * view_matrix; //must multiply in this order
	}






	// PerspectiveCamera::PerspectiveCamera(float width, float height)
	// 	: Camera(Perspective(width/-2, width/2, height/-2, height/2, -1.0f, 1.0f)){

	// }

}
