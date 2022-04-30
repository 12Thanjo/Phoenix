#pragma once


namespace Phoenix{

	class RenderContext{
		private:
	
		public:
			virtual void init() = 0;
			virtual void swapBuffers() = 0;
	};
	
}

