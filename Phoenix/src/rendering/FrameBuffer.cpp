#include "ph_pch.h"
#include "FrameBuffer.h"


#include <glad/glad.h>


namespace Phoenix{

	FrameBuffer::FrameBuffer(const FrameBufferConfig& _config)
		: config(_config) {

		invalidate();
	}


	FrameBuffer::~FrameBuffer(){
		glDeleteFramebuffers(1, &renderer_id);
		glDeleteTextures(1, &color_attachment);
		glDeleteTextures(1, &depth_attachment);
	}



	void FrameBuffer::invalidate(){
		if(renderer_id){
			glDeleteFramebuffers(1, &renderer_id);
			glDeleteTextures(1, &color_attachment);
			glDeleteTextures(1, &depth_attachment);
		}

		glCreateFramebuffers(1, &renderer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);

		glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment);
		glBindTexture(GL_TEXTURE_2D, color_attachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, config.width, config.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);

		// depth buffer
		glCreateTextures(GL_TEXTURE_2D, 1, &depth_attachment);
		glBindTexture(GL_TEXTURE_2D, depth_attachment);
		// glTextureStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, config.width, config.height);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, config.width, config.height,
				0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_attachment, 0);


		// check status
		PH_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete");


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind(){
		glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);
		glViewport(0, 0, config.width, config.height);
	}

	void FrameBuffer::unbind(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::resize(uint32_t width, uint32_t height){
		config.width = width;
		config.height = height;
		invalidate();
	}

	
}
