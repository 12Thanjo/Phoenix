#pragma once


namespace Phoenix{

	enum class FrameBufferTextureFormat{
		None = 0,


		// color
		RGBA8,
		// DepthColor,

		// depth/stencis
		DEPTH24STENCIL8,

		// defaults
		Depth = DEPTH24STENCIL8
	};


	struct FrameBufferTextureConfig{
		FrameBufferTextureConfig() = default;
		FrameBufferTextureConfig(FrameBufferTextureFormat format)
			: textureFormat(format){

		};

		FrameBufferTextureFormat textureFormat = FrameBufferTextureFormat::None;

		//////////////////////////
		// TODO: filtering/wrap //
		//////////////////////////
	};




	struct FrameBufferAttachmentConfig{
		FrameBufferAttachmentConfig() = default;
		FrameBufferAttachmentConfig(std::initializer_list<FrameBufferTextureConfig> texture_attachments)
			: attachments(texture_attachments) {

		}

		std::vector<FrameBufferTextureConfig> attachments;
	};



	///////////////////////////////////



	struct FrameBufferConfig{
		unsigned int width = 0;
		unsigned int height = 0;

		FrameBufferAttachmentConfig attachments;
		unsigned int samples = 1;

		// bool swapChainTarget = false;
	};



	class FrameBuffer{
		private:
			glID _id;
			// glID _color_attachment;
			// glID _depth_attachment;//////////////
			FrameBufferConfig _config;

			std::vector<FrameBufferTextureConfig> _color_attachment_configs;
			FrameBufferTextureConfig _depth_attachment_config;

			std::vector<glID> _color_attachments;
			glID _depth_attachment;
	
		public:
			FrameBuffer(const FrameBufferConfig& config);
			~FrameBuffer();



			void bind();

			void textureBind();
			void textureBind(int i);

			void unbind();



			void resize(unsigned int width, unsigned int height);

			inline glID getID() const { return _id; }
			inline glID getColorAttachment(int i){ return _color_attachments[i]; }
			inline glID getDepthAttachment(){ return _depth_attachment; }
			inline unsigned int getWidth() const { return _config.width; }
			inline unsigned int getHeight() const { return _config.height; }

		private:
			void _rebuild();
			
	};
	
}
