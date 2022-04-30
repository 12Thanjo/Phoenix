#pragma once

#include "renderer/buffer.h"

namespace Phoenix{
	
	class OpenGLVertexBuffer : public VertexBuffer{
		private:
			uint32_t renderer_id;
			BufferLayout layout;

		public:
			OpenGLVertexBuffer(float* verticies, uint32_t size);
			virtual ~OpenGLVertexBuffer();

			void bind() const override;
			void unbind() const override;

			inline const BufferLayout& getLayout() override { return layout; };
			inline void setLayout(const BufferLayout& _layout) override { layout = _layout; };

	};


	class OpenGLIndexBuffer : public IndexBuffer{
		private:
			uint32_t renderer_id;
			uint32_t count;

		public:
			OpenGLIndexBuffer(uint32_t* verticies, uint32_t _count);
			virtual ~OpenGLIndexBuffer();

			void bind() const override;
			void unbind() const override;

			uint32_t getCount() const override { return count; };
	};

}
