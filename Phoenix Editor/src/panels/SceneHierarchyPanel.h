#pragma once

// #include <Phoenix.h>


#include "Panel.h"
#include "src/ECS/Entity.h"



namespace Phoenix{

	enum struct SHP_State{
		None,
		Grab,
		Rotate,
		Scale
	};

	
	class SceneHierarchyPanel : public Panel{
		public:
			SceneHierarchyPanel() = default;
			~SceneHierarchyPanel() = default;

			void render(Engine* editor) override;
			void onEvent(Event& e, Engine* editor) override;

		public:
			Entity selection_context;
			SHP_State current_state = SHP_State::None;

			bool scene_hierarchy_visible = true;
			bool entity_properties_visible = true;

		private:
			void draw_entity_node(Engine* editor, Entity& entity);
			void draw_components(Engine* editor, Entity& entity);

			void reset_property();
			void finish_tool();
			bool is_using_tool();

		private:
			float _mouse_x = 0;
			float _mouse_y = 0;

			bool _can_adjust_x = true;
			bool _can_adjust_y = true;
			bool _can_adjust_z = true;

			glm::vec3 _original_vec3;
	};

}
