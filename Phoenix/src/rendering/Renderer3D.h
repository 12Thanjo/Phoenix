#pragma once


namespace Phoenix{
	namespace Lights{
		class Directional;
		class Point;
	};
	class AssetManager;
	class PerspectiveCamera;
	class BasicMaterial;

	struct Renderer3DPerformanceMetrics{
		unsigned int drawCalls = 0;
		unsigned int verticies = 0;
		unsigned int indicies = 0;
	};

	class Renderer3D{
		public:
			Renderer3D(AssetManager* asset_manager);
			~Renderer3D() = default;
		
			void init();
			void bindBasic(PerspectiveCamera& camera, Lights::Directional& sunlight);
			void bindPointLight(Lights::Point& light, glm::vec3& position);
			void finishBindBasic();
			void bindCard(PerspectiveCamera& camera);

			void drawCube(uint32_t entt_id, glm::mat4& transform, BasicMaterial& material);
			void drawPlane(uint32_t entt_id, glm::mat4& transform, BasicMaterial& material);

			void drawCard(uint32_t entt_id, glm::mat4& transform, UUID& texture);

			void resetPerfMetrics();

		public:
			Renderer3DPerformanceMetrics performanceMetrics;

		private:
			void basicMaterialUpload(uint32_t entt_id, glm::mat4& transform, BasicMaterial& material);

		private:
			AssetManager* _asset_manager;

			UUID _basic_shader;
			UUID _card_shader;

			int _light_index = 0;

	};

}
