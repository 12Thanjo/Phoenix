#pragma once

#include <glad/glad.h>

namespace Phoenix{

	struct VertexBufferElement{
		unsigned int type;
		unsigned int count;
		unsigned int size;
	};

	class VertexBuffer{
		private:
			glID _id;
			std::vector<VertexBufferElement> _layout;
			unsigned int _stride = 0;
	
		public:
			VertexBuffer(float* verticies, unsigned int size);
			~VertexBuffer();

			void bind();
			void unbind();


			inline unsigned int getStride() const { return _stride; }
			inline std::vector<VertexBufferElement> getLayout() const { return _layout; }


			///////////////////
			// with count
			template<typename T>
			void layout(unsigned int count){
				static_assert(sizeof(T) == 0, "vertex buffer layout type not supported");
			};

			template<>
			void layout<float>(unsigned int count){
				PH_ASSERT(count < 5, "Largest size for floats in a vertex buffer layout is 4");

				unsigned int elem_size = sizeof(float) * count;

				_layout.push_back( {GL_FLOAT, count, elem_size} );
				_stride += elem_size;
			};

			template<>
			void layout<int>(unsigned int count){
				PH_ASSERT(count < 5, "Largest size for ints in a vertex buffer layout is 4");

				unsigned int elem_size = sizeof(int) * count;

				_layout.push_back( {GL_INT, count, elem_size} );
				_stride += elem_size;
			};

			// without count
			template<typename T>
			void layout(){
				static_assert(sizeof(T) == 0, "vertex buffer layout type not supported");
			};

			template<>
			void layout<glm::mat3>(){
				unsigned int elem_size = sizeof(float) * 4 * 3;

				_layout.push_back( {GL_INT, 3 * 4, elem_size} );
				_stride += elem_size;	
			};

			template<>
			void layout<glm::mat4>(){
				unsigned int elem_size = sizeof(float) * 4 * 4;

				_layout.push_back( {GL_INT, 4 * 4, elem_size} );
				_stride += elem_size;	
			};


			template<>
			void layout<bool>(){
				_layout.push_back( {GL_INT, 1, 1} );
				_stride += 1;	
			};
			
	};



	class IndexBuffer{
		private:
			glID _id;
	
		public:
			IndexBuffer(unsigned int* indicies, unsigned int size);
			~IndexBuffer();

			void bind();
			void unbind();
			
	};


	class VertexArray{
		private:
			glID _id;
	
		public:
			VertexArray();
			~VertexArray();

			void bind();
			void unbind();


			void addVertexBuffer(VertexBuffer& vertex_buffer);

	};
	
}
