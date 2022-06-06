#include "ph_pch.h"
#include "buffers.h"


#include <glad/glad.h>

namespace Phoenix{

	VertexBuffer::VertexBuffer(float* verticies, unsigned int size){
		glCreateBuffers(1, &_id);
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}


	VertexBuffer::~VertexBuffer(){
		glDeleteBuffers(1, &_id);
	}


	void VertexBuffer::bind(){
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	void VertexBuffer::unbind(){
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	////////////////////////////////////////////////////////////////////////////

	
	IndexBuffer::IndexBuffer(unsigned int* indicies, unsigned int size){
		glGenBuffers(1, &_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STATIC_DRAW);
	}


	IndexBuffer::~IndexBuffer(){
		glDeleteBuffers(1, &_id);
	}


	void IndexBuffer::bind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	}

	void IndexBuffer::unbind(){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}



	////////////////////////////////////////////////////////////////////////////


	VertexArray::VertexArray(){
		glGenVertexArrays(1, &_id);
		glBindVertexArray(_id);
	}


	VertexArray::~VertexArray(){
		glDeleteVertexArrays(1, &_id);

	}


	void VertexArray::bind(){
		glBindVertexArray(_id);
	}

	void VertexArray::unbind(){
		glBindVertexArray(0);
	}


	void VertexArray::addVertexBuffer(VertexBuffer& vertex_buffer){
		glBindVertexArray(_id);
		vertex_buffer.bind();

		PH_ASSERT(vertex_buffer.getLayout().size() != 0, "Vertex buffer is empty");


		std::vector<VertexBufferElement> layout = vertex_buffer.getLayout();

		unsigned int index = 0;
		unsigned int offset = 0;
		unsigned int stride = vertex_buffer.getStride();

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
	}

}
