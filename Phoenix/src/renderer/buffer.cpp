#include "ph_pch.h"
#include "buffer.h"

#include "Renderer.h"
#include "platform/OpenGL/OpenGL buffer.h"

namespace Phoenix{

	VertexBuffer* VertexBuffer::create(float* verticies, uint32_t size){
		switch(Renderer::getAPI()){			
			case RendererAPI::None:		ph_internal_fatal("RendererAPI::None is not currently supported"); return nullptr;
			case RendererAPI::OpenGL: 	return new OpenGLVertexBuffer(verticies, size);
		}

		ph_internal_fatal("Unknown RendererAPI");
		return nullptr;
	}


	IndexBuffer* IndexBuffer::create(uint32_t* indicies, uint32_t count){
		switch(Renderer::getAPI()){			
			case RendererAPI::None:		ph_internal_fatal("RendererAPI::None is not currently supported"); return nullptr;
			case RendererAPI::OpenGL: 	return new OpenGLIndexBuffer(indicies, count);
		}

		ph_internal_fatal("Unknown RendererAPI");
		return nullptr;
	}

	
}
