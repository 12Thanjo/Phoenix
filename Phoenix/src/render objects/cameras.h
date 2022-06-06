#pragma once


namespace Phoenix{
	
	class Camera{
		public:
			Camera(float fov, float aspect_ratio, float near, float far);
			~Camera() = default;

			void moveForward(float amount);
			void moveBackward(float amount);
			void moveLeft(float amount);
			void moveRight(float amount);
			void moveUp(float amount);
			void moveDown(float amount);

			void setProjection(float fov, float aspect_ratio, float near, float far);
			glm::mat4 getViewProjection();


			inline glm::vec3 getPosition() const { return _position; }
			void setPosition(glm::vec3 position);

			inline glm::vec3 getRotation() const { return _rotation; }
			void setRotation(glm::vec3 rotation);

			inline float getFOV() const { return _fov; }
			inline float getAspectRatio() const { return _aspect_ratio; }
			inline float getNear() const { return _near; }
			inline float getFar() const { return _far; }

		private:
			glm::vec3 _front = {1.0f, 0.0f, 0.0f};
			glm::vec3 _up = {0.0f, 1.0f, 0.0f};


			// user controlled
			glm::vec3 _rotation = {0.0f, 0.0f, 0.0f};
			glm::vec3 _position = {0.0f, 0.0f, 0.0f};

			float _fov;
			float _aspect_ratio;
			float _near;
			float _far;

			glm::mat4 _view;
			glm::mat4 _projection;

		private:
			void calculate_view();
			void calculate_projection();

	};

}
