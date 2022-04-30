#include "ph_pch.h"
#include "OpenGL buffer.h"

#include <glad/glad.h>

namespace Phoenix{

	// Vertex Buffer /////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size){
		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);

	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer(){
		glDeleteBuffers(1, &renderer_id);
	}

	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, renderer_id);
	}

	void OpenGLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}



	// Index Buffer //////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indicies, uint32_t _count)
		: count(_count) {
		glCreateBuffers(1, &renderer_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indicies, GL_STATIC_DRAW);

	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer(){
		glDeleteBuffers(1, &renderer_id);
	}

	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id);
	}

	void OpenGLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}
