#pragma once

// #include <Phoenix.h>


#include "Panel.h"
#include "src/ECS/Entity.h"



namespace Phoenix{
	
	class SceneHierarchyPanel : public Panel{
		private:
			Entity _selection_context;
	
		public:
			SceneHierarchyPanel() = default;
			~SceneHierarchyPanel() = default;

			void render(Engine* editor) override;

		private:
			void draw_entity_node(Engine* editor, Entity& entity);
			void draw_components(Engine* editor, Entity& entity);
	};

}
