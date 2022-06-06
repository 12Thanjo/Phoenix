#pragma once


namespace Phoenix{
	
	class AssetManager;
	class Camera;

	class Renderer2D{
		private:
			UUID _quad_shader;
			AssetManager* _asset_manager;
	
		public:
			Renderer2D(AssetManager* asset_manager);
			~Renderer2D();
			
			void init();

			void drawQuad(glm::mat4& transform, glm::vec4& color, Camera& camera);


	};

}
