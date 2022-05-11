#pragma once

#include <glad/glad.h>

namespace Phoenix{

	struct VertexBufferElement{
		uint32_t type;
		uint32_t count;
		uint32_t size;
	};

	class VertexBuffer{
		private:
			RendererID renderer_id;
			std::vector<VertexBufferElement> layout;
			uint32_t stride = 0;
	
		public:
			VertexBuffer(float* verticies, uint32_t size);
			VertexBuffer(uint32_t size);
			~VertexBuffer();

			void bind();
			void unbind();

			void set_data(const void* data, uint32_t size);


			inline uint32_t get_stride() { return stride; }
			inline std::vector<VertexBufferElement> get_layout() const { return layout; }


			///////////////////
			// with count
			template<typename T>
			void push(uint32_t count){
				static_assert(sizeof(T) == 0, "vertex buffer layout type not supported");
			};

			template<>
			void push<float>(uint32_t count){
				PH_ASSERT(count < 5, "Largest size for floats in a vertex buffer layout is 4");

				uint32_t elem_size = sizeof(float) * count;

				layout.push_back( {GL_FLOAT, count, elem_size} );
				stride += elem_size;
			};

			template<>
			void push<int>(uint32_t count){
				PH_ASSERT(count < 5, "Largest size for ints in a vertex buffer layout is 4");

				uint32_t elem_size = sizeof(int) * count;

				layout.push_back( {GL_INT, count, elem_size} );
				stride += elem_size;
			};

			// without count
			template<typename T>
			void push(){
				static_assert(sizeof(T) == 0, "vertex buffer layout type not supported");
			};

			template<>
			void push<glm::mat3>(){
				uint32_t elem_size = sizeof(float) * 4 * 3;

				layout.push_back( {GL_INT, 3 * 4, elem_size} );
				stride += elem_size;	
			};

			template<>
			void push<glm::mat4>(){
				uint32_t elem_size = sizeof(float) * 4 * 4;

				layout.push_back( {GL_INT, 4 * 4, elem_size} );
				stride += elem_size;	
			};


			template<>
			void push<bool>(){
				layout.push_back( {GL_INT, 1, 1} );
				stride += 1;	
			};

	};


	class IndexBuffer{
		private:
			RendererID renderer_id;
	
		public:
			IndexBuffer(Ref<uint32_t> indicies, uint32_t count);
			~IndexBuffer();

			void bind();
			void unbind();
	};




	////////////////////////////////////////////////////////////////////////////////////////////


	class VertexArray{
		private:
			RendererID renderer_id;
			// std::vector<Ref<VertexBuffer>> vertex_buffers;
			// Ref<IndexBuffer> index_buffer;
	
		public:
			VertexArray();
			~VertexArray();

			void bind();
			void unbind();

			void add_vertex_buffer(const Ref<VertexBuffer>& vertex_buffer);
			void set_index_buffer(const Ref<IndexBuffer>& index_buffer);
	};
	
}
