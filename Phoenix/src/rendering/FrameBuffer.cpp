#include "ph_pch.h"
#include "FrameBuffer.h"


#include <glad/glad.h>
// #include <GLFW/glfw3.h>


namespace Phoenix{

	static const unsigned int MAX_FRAMEBUFFER_SIZE = 8192;

	static bool is_depth_format(FrameBufferTextureFormat& format){
		switch(format){
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
		}


		return false;
	};


	static GLenum convert_texture_format(FrameBufferTextureFormat format){
		switch(format){
			case FrameBufferTextureFormat::RGBA8: 		return GL_RGBA8;
			case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
		};

		PH_ASSERT(false, "Unknown format");
		return 0;
	}


	static GLenum texture_target(bool multisampled){
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}


	static void create_textures(bool multisampled, glID* out_id, unsigned int count){
		glCreateTextures(texture_target(multisampled), count, out_id);
	}

	static void bind_texture(bool multisampled, glID id){
		glBindTexture(texture_target(multisampled), id);
	}



	static void attatch_color_texture(glID id, int samples, GLenum format, GLenum output_format, GLenum type, unsigned int width, unsigned int height, int index){
		bool multisampled = samples > 1;

		if(multisampled){
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}else{
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, output_format, type, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		};


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, texture_target(multisampled), id, 0);
	}



	static void attach_depth_texture(glID id, int samples, GLenum format, GLenum attachment_type, unsigned int width, unsigned int height){
		bool multisampled = samples > 1;

		if(multisampled){
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}else{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		};


		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_type, texture_target(multisampled), id, 0);
	}



	////////////////////////////////////////////////////////////////////////////////////////

	FrameBuffer::FrameBuffer(const FrameBufferConfig& config)
		: _config(config) {


		for(auto& format : _config.attachments.attachments){
			if(!is_depth_format(format.textureFormat)){
				_color_attachment_configs.emplace_back(format);
			}else{
				_depth_attachment_config = format;
			};
		}



		_rebuild();
	}


	FrameBuffer::~FrameBuffer(){
		glDeleteFramebuffers(1, &_id);
		glDeleteTextures((GLsizei)_color_attachments.size(), _color_attachments.data());
		glDeleteTextures(1, &_depth_attachment);
	}



	void FrameBuffer::_rebuild(){
		if(_id){
			glDeleteFramebuffers(1, &_id);
			glDeleteTextures((GLsizei)_color_attachments.size(), _color_attachments.data());
			glDeleteTextures(1, &_depth_attachment);

			_color_attachments.clear();
			_depth_attachment = 0;
		}

		glCreateFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);


		bool multisample = _config.samples > 1;


		// attachments ///////////
		if(_color_attachment_configs.size()){

			_color_attachments.resize(_color_attachment_configs.size());
			create_textures(multisample, _color_attachments.data(), (unsigned int)_color_attachments.size());


			for(size_t i = 0; i < _color_attachments.size(); i += 1){
				bind_texture(multisample, _color_attachments[i]);

				switch(_color_attachment_configs[i].textureFormat){
					case FrameBufferTextureFormat::RGBA8: 
						attatch_color_texture(_color_attachments[i], _config.samples, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, (GLint)_config.width, (GLint)_config.height, (int)i);
						break;
					case FrameBufferTextureFormat::RED_INTEGER:
						attatch_color_texture(_color_attachments[i], _config.samples, GL_R32I, GL_RED_INTEGER, GL_UNSIGNED_BYTE, (GLint)_config.width, (GLint)_config.height, (int)i);
						break;
					// case FrameBufferTextureFormat::DepthColor: 
					// 	attatch_color_texture(_color_attachments[i], _config.samples, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT,  (GLint)_config.width, (GLint)_config.height, (int)i);
						// glDrawBuffer(GL_NONE);
						// glReadBuffer(GL_NONE);
						// break;
				}
			}
		}


		if(_depth_attachment_config.textureFormat != FrameBufferTextureFormat::None){
			create_textures(multisample, &_depth_attachment, 1);
			bind_texture(multisample, _depth_attachment);

			switch(_depth_attachment_config.textureFormat){
				case FrameBufferTextureFormat::DEPTH24STENCIL8:
					attach_depth_texture(_depth_attachment, _config.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, (GLint)_config.width, (GLint)_config.height);
					break;
			}
		}


		if(_color_attachments.size() > 1){
			PH_ASSERT(_color_attachments.size() <= 2, "A maximum of 2 color attachments per FrameBuffer is suppoerted");

			GLenum buffers[4] {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
			glDrawBuffers((GLsizei)_color_attachments.size(), buffers);
		}else if(_color_attachments.empty()){
			glDrawBuffer(GL_NONE);
		};




		// check status
		PH_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame buffer is incomplete");


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void FrameBuffer::bind(){
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glViewport(0, 0, (GLint)_config.width, (GLint)_config.height);
	}

	// void FrameBuffer::textureBind(){
	// 	glActiveTexture(GL_TEXTURE0); // makes the engine render the buffer, and the last loaded texture
	// 	glBindTexture(GL_TEXTURE_2D, _id);
	// }

	// void FrameBuffer::textureBind(int i){
	// 	glActiveTexture(GL_TEXTURE0); // makes the engine render the buffer, and the last loaded texture
	// 	glBindTexture(GL_TEXTURE_2D, _color_attachments[i]);
	// }


	void FrameBuffer::clearAttachment(glID attachment_index, int value){
		PH_ASSERT(attachment_index < _color_attachments.size(), "invalid attachment index");

		auto& config = _color_attachment_configs[attachment_index];
		glClearTexImage(_color_attachments[attachment_index], 0, convert_texture_format(config.textureFormat), GL_INT, &value);
	}


	void FrameBuffer::unbind(){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::resize(unsigned int width, unsigned int height){
		_config.width = width;
		_config.height = height;
		_rebuild();
	}
	
}
