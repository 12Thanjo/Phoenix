#pragma once


namespace Phoenix{

	struct FrameBufferConfig {
		uint32_t width, height;
	};
	
	class FrameBuffer{
		private:
			FrameBufferConfig config;
			RendererID renderer_id = 0;
			RendererID color_attachment = 0;
			RendererID depth_attachment = 0;
	
		public:
			FrameBuffer(const FrameBufferConfig& _config);
			~FrameBuffer();


			void invalidate();
			void bind();
			void unbind();
			void resize(uint32_t width, uint32_t height);


			inline RendererID get_color_attachment() const { return color_attachment; }
			
	};

}
