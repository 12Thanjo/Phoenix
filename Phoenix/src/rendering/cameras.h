#pragma once


namespace Phoenix{
	
	class Camera{
		protected:
			glm::mat4 projection_matrix;
			glm::mat4 view_matrix = glm::mat4(1.0f);
			glm::mat4 view_projection_matrix;

	
		public:
			Camera(glm::mat4 ortho);
			// Camera(Perspective perspective);

			// const glm::mat4& get_projection_matrix() const { return projection_matrix; }
			// const glm::mat4& get_view_matrix() const { return view_matrix; }
			const glm::mat4& get_view_projection_matrix() const { return view_projection_matrix; }

	};


	class OrthographicCamera : public Camera{
		private:
			glm::vec2 position = {0.0f, 0.0f};
			float rotation = 0.0f;
	
		public:
			OrthographicCamera(float width, float height);


			glm::vec2 get_position() const { return position; };
			inline void set_position(const glm::vec2& _position){ position = _position; calculate_view_matrix(); };

			float get_rotation() const { return rotation; };
			inline void set_rotation(const float& _rotation){ rotation = _rotation; calculate_view_matrix(); };


			void set_projection(float width, float height);
		
		private:
			void calculate_view_matrix();
	};


	// class PerspectiveCamera : public Camera{
	// 	private:
	// 		Vec3 position = {0.0f, 0.0f, 0.0f};
	// 		Vec3 rotation = {0.0f, 0.0f, 0.0f};
	
	// 	public:
	// 		PerspectiveCamera(float width, float height);
			
	// };

}
