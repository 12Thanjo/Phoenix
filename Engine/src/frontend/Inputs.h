#pragma once

#include "../../include/Phoenix/defines.h"
#include "./EngineInterface.h"

#include "../inputs/codes.h"

namespace ph{


	
	class Inputs{
		public:
			using Mouse = ph::InputCodes::Mouse;
			using Key = ph::InputCodes::Key;

			Inputs(EngineInterface* interface_ptr) : interface(interface_ptr) {};
			~Inputs() = default;



			PH_NODISCARD inline auto isDown(Mouse button) noexcept -> bool {
				return this->interface->is_mouse_down(static_cast<int>(button));
			};

			PH_NODISCARD inline auto isUp(Mouse button) noexcept -> bool {
				return this->interface->is_mouse_up(static_cast<int>(button));
			};



			PH_NODISCARD inline auto isDown(Key key) noexcept -> bool {
				return this->interface->is_key_down(static_cast<int>(key));
			};

			PH_NODISCARD inline auto isUp(Key key) noexcept -> bool {
				return this->interface->is_key_up(static_cast<int>(key));
			};



			PH_NODISCARD inline auto mouseX() noexcept -> int32_t { return this->interface->mouse_x(); };
			PH_NODISCARD inline auto mouseY() noexcept -> int32_t { return this->interface->mouse_y(); };

			PH_NODISCARD inline auto mouseDX() noexcept -> int32_t { return this->interface->mouse_dx(); };
			PH_NODISCARD inline auto mouseDY() noexcept -> int32_t { return this->interface->mouse_dy(); };


	
		private:
			EngineInterface* interface;
	};

};
