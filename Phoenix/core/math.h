// #pragma once

// #include <lib/glm/glm/glm.hpp>
// #include <lib/glm/glm/gtc/matrix_transform.hpp>



// #define GLM_TYPE_CONVERT(t) operator glm::t() const { return val; }
// #define COUT_EXTEND(T, x) friend std::ostream& operator << (std::ostream& os, T& self){ return os << x; }

// namespace Phoenix{


// 	class Vec2{
// 		private:
// 			glm::vec2 val;
	
// 		public:
// 			Vec2() : val(1.0f) {};
// 			Vec2(glm::vec2 convert) : val(convert) {};
// 			Vec2(auto num) : val((float)num) {};
//  			Vec2(auto num1, auto num2) : val((float)num1, (float)num2) {};

//  			GET_SET(float, x, val.x);
//  			GET_SET(float, y, val.y);


// 			GLM_TYPE_CONVERT(vec2);
// 			COUT_EXTEND(Vec2, "{ Vec2 (" << self.val.x << ", " << self.val.y << ") }");
// 	};

	
// 	class Vec3{
// 		private:
// 			glm::vec3 val;
	
// 		public:
// 			Vec3() : val(1.0f) {};
// 			Vec3(glm::vec3 convert) : val(convert) {};
// 			Vec3(auto num) : val((float)num) {};
// 			Vec3(auto num1, auto num2, auto num3) : val((float)num1, (float)num2, (float)num3) {};

// 			GET_SET(float, x, val.x);
// 			GET_SET(float, y, val.y);
// 			GET_SET(float, z, val.z);


// 			GLM_TYPE_CONVERT(vec3);
// 			COUT_EXTEND(Vec3, "{ Vec3 (" << self.val.x << ", " << self.val.y << ", " << self.val.z  << ") }");
// 	};



// 	class Vec4{
// 		private:
// 			glm::vec4 val;
	
// 		public:
// 			Vec4() : val(1.0f) {};
// 			Vec4(glm::vec4 convert) : val(convert) {};
// 			Vec4(auto num) : val((float)num) {};
// 			Vec4(auto num1, auto num2, auto num3, auto num4) : val((float)num1, (float)num2, (float)num3, (float)num4) {};

// 			GET_SET(float, x, val.x);
// 			GET_SET(float, y, val.y);
// 			GET_SET(float, z, val.z);
// 			GET_SET(float, w, val.w);

// 			GET_SET(float, r, val.r);
// 			GET_SET(float, g, val.g);
// 			GET_SET(float, b, val.b);
// 			GET_SET(float, a, val.a);


// 			GLM_TYPE_CONVERT(vec4);
// 			COUT_EXTEND(Vec4, "{ Vec4 (" << self.val.x << ", " << self.val.y << ", " << self.val.z << ", " << self.val.w << ") }");
// 	}; 



// 	class Mat3{
// 		private:
// 			glm::mat3 val;
	
// 		public:
// 			Mat3() : val(1.0f) {};
// 			Mat3(glm::mat3 convert) : val(convert) {};


// 			GLM_TYPE_CONVERT(mat3);
// 			COUT_EXTEND(Mat3, "{ Mat3 }");
// 	};



// 	class Mat4{
// 		private:
// 			glm::mat4 val;
	
// 		public:
// 			Mat4() : val(1.0f) {};
// 			Mat4(glm::mat4 convert) : val(convert) {};


// 			GLM_TYPE_CONVERT(mat4);
// 			COUT_EXTEND(Mat4, "{ Mat4 }");



// 			Mat4 operator * (const Mat4& other){
// 				return Mat4(val * (glm::mat4)other);
// 			}
// 	};



// 	class Orthographic{
// 		private:
// 			glm::mat4 val;
	
// 		public:
// 			Orthographic(auto left, auto right, auto bottom, auto top, auto near, auto far)
// 				: val(glm::ortho((float)left, (float)right, (float)bottom, (float)top, (float)near, (float)far)) {}

// 			GLM_TYPE_CONVERT(mat4);
// 	};



// 	// class Perspective{
// 	// 	private:
// 	// 		glm::mat4 val;
	
// 	// 	public:
// 	// 		Perspective(auto left, auto right, auto bottom, auto top, auto near, auto far)
// 	// 			: val(glm::perspective((float)left, (float)right, (float)bottom, (float)top, (float)near, (float)far)) {}

// 	// 		GLM_TYPE_CONVERT(mat4);
// 	// };



// }

// #undef GLM_TYPE_CONVERT
// #undef COUT_EXTEND

