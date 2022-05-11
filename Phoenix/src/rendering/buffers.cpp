#include "ph_pch.h"
#include "buffers.h"

#include <glad/glad.h>

namespace Phoenix{

	VertexBuffer::VertexBuffer(float* verticies, uint32_t size)
		: stride(0){

		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	};


	VertexBuffer::VertexBuffer(uint32_t size)
		: stride(0){

		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}



	VertexBuffer::~VertexBuffer(){
		glDeleteBuffers(1, &renderer_id);
	}




	void VertexBuffer::bind(){
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	}

	void VertexBuffer::unbind(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	void VertexBuffer::set_data(const void* data, uint32_t size){
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}


	/////////////////////////////////////////////////////////////////


	IndexBuffer::IndexBuffer(Ref<uint32_t> indicies, uint32_t count){
		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies.get(), GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer(){
		glDeleteBuffers(1, &renderer_id);
	}




	void IndexBuffer::bind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	}

	void IndexBuffer::unbind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


	/////////////////////////////////////////////////////////////////


	VertexArray::VertexArray(){
		// glCreateVertexArrays(1, &renderer_id);
		glGenVertexArrays(1, &renderer_id);
		glBindVertexArray(renderer_id);
	}

	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &renderer_id);
	}





	void VertexArray::bind(){
		glBindVertexArray(renderer_id);
	}

	void VertexArray::unbind(){
		glBindVertexArray(0);
	}





	void VertexArray::add_vertex_buffer(const Ref<VertexBuffer>& vertex_buffer){
		glBindVertexArray(renderer_id);
		vertex_buffer->bind();

		PH_ASSERT(vertex_buffer->get_layout().size() != 0, "Vertex buffer is empty");


		std::vector<VertexBufferElement> layout = vertex_buffer->get_layout();

		uint32_t index = 0;
		uint32_t offset = 0;
		uint32_t stride = vertex_buffer->get_stride();

		for(auto& element : layout){
			glEnableVertexAttribArray(index);

			glVertexAttribPointer(
				index,
				element.count,
				element.type,
				GL_FALSE,
				stride,
				(const void*)offset
			);

			offset += element.size;

			index += 1;
		}

		// vertex_buffers.push_back(vertex_buffer);

	}

	void VertexArray::set_index_buffer(const Ref<IndexBuffer>& _index_buffer){
		glBindVertexArray(renderer_id);
		_index_buffer->bind();

		// index_buffer = _index_buffer;
	}

	
}
