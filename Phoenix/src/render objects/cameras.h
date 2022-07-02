#pragma once



namespace Phoenix{
	class Environment;

	class Camera{
		public:
			Camera() = default;
			~Camera() = default;


			glm::mat4 getViewProjection(){ return _projection * _view; }

	
		protected:
			glm::mat4 _view;
			glm::mat4 _projection;
	};
	
	class PerspectiveCamera : public Camera{
		public:
			PerspectiveCamera(float fov, float aspect_ratio, float near, float far);
			~PerspectiveCamera(){};

			void moveForward(float amount);
			void moveBackward(float amount);
			void moveLeft(float amount);
			void moveRight(float amount);
			void moveUp(float amount);
			void moveDown(float amount);

			void setProjection(float fov, float aspect_ratio, float near, float far);
			// glm::mat4 getViewProjection();


			inline glm::vec3 getPosition() const { return _position; }
			void setPosition(glm::vec3 position);

			inline glm::vec3 getRotation() const { return _rotation; }
			void setRotation(glm::vec3 rotation);


			inline float getFOV() const { return _fov; }
			inline float getAspectRatio() const { return _aspect_ratio; }
			inline float getNear() const { return _near; }
			inline float getFar() const { return _far; }


			inline glm::vec3 getFront() const { return _front; };
			inline glm::vec3 getUp() const { return _up; };

		private:
			glm::vec3 _position = {0.0f, 0.0f, 0.0f};
			glm::vec3 _rotation = {0.0f, 0.0f, 0.0f};

			float _fov;
			float _aspect_ratio;
			float _near;
			float _far;

			glm::vec3 _front = {1.0f, 0.0f, 0.0f};
			glm::vec3 _up = {0.0f, 1.0f, 0.0f};


		private:
			void calculate_view();
			void calculate_projection();
	};


	class OrbitalCamera : public Camera{
		public:
			OrbitalCamera(float fov = glm::radians(65.0f), float aspect_ratio = 1.0f, float near = 0.1f, float far = 100.0f);
			~OrbitalCamera(){};

			void setProjection(float fov, float aspect_ratio, float near, float far);
			// glm::mat4 getViewProjection();


			inline glm::vec3 getPosition() const { return _position; }
			void setPosition(glm::vec3 position);

			inline glm::vec3 getRotation() const { return _rotation; }
			void setRotation(glm::vec3 rotation);

			inline glm::vec3 getFocalPoint() const { return _focal_point; }
			void setFocalPoint(glm::vec3 focal_point);


			inline float getRho() const { return _rho; }
			inline float getTheta() const { return _theta; }
			inline float getPhi() const { return _phi; }

			void setCoordinates(float rho, float theta, float phi);

			inline float getFOV() const { return _fov; }
			inline float getAspectRatio() const { return _aspect_ratio; }
			inline float getNear() const { return _near; }
			inline float getFar() const { return _far; }


			inline glm::vec3 getFront() const { return _front; };
			inline glm::vec3 getUp() const { return _up; };

		private:
			glm::vec3 _position = {0.0f, 0.0f, 0.0f};
			glm::vec3 _rotation = {0.0f, 0.0f, 0.0f};

			glm::vec3 _focal_point{0.0f};
			float _rho = 5.0f;
			float _theta = 3.14f*0.5;
			float _phi = 3.14f*0.25;

			float _fov;
			float _aspect_ratio;
			float _near;
			float _far;

			glm::vec3 _front = {1.0f, 0.0f, 0.0f};
			glm::vec3 _up = {0.0f, 1.0f, 0.0f};


		private:
			void calculate_view();
			void calculate_projection();
			void place();
	};

}
