#pragma once


#include <libs/evo/evo.h>


#include "inputs/codes.h"

namespace ph{
	namespace GLFW{
		class Window;
	};


	class InputManager{
		public:
			InputManager() = default;
			~InputManager() = default;

			EVO_NODISCARD auto init(GLFW::Window& window) noexcept -> bool;
			auto shutdown() noexcept -> void;

			auto update() noexcept -> void;


			EVO_NODISCARD auto is_mouse_down(InputCodes::Mouse button) const noexcept -> bool;
			EVO_NODISCARD auto is_mouse_up(InputCodes::Mouse button) const noexcept -> bool;

			EVO_NODISCARD auto was_mouse_down(InputCodes::Mouse button) const noexcept -> bool;
			EVO_NODISCARD auto was_mouse_up(InputCodes::Mouse button) const noexcept -> bool;


			EVO_NODISCARD auto is_key_down(InputCodes::Key key) const noexcept -> bool;
			EVO_NODISCARD auto is_key_up(InputCodes::Key key) const noexcept -> bool;

			EVO_NODISCARD auto was_key_down(InputCodes::Key key) const noexcept -> bool;
			EVO_NODISCARD auto was_key_up(InputCodes::Key key) const noexcept -> bool;


			EVO_NODISCARD inline auto get_mouse_x() noexcept -> int32_t { return this->mouse.x; };
			EVO_NODISCARD inline auto get_mouse_y() noexcept -> int32_t { return this->mouse.y; };

			EVO_NODISCARD inline auto get_mouse_dx() noexcept -> int32_t { return this->mouse.dx; };
			EVO_NODISCARD inline auto get_mouse_dy() noexcept -> int32_t { return this->mouse.dy; };

			EVO_NODISCARD inline auto get_mouse_wheel_dx() noexcept -> int32_t { return this->mouse.wheel.dx; };
			EVO_NODISCARD inline auto get_mouse_wheel_dy() noexcept -> int32_t { return this->mouse.wheel.dy; };

	
		private:
			std::array<bool, static_cast<size_t>(InputCodes::Key::_size)> key_down{};
			std::array<bool, static_cast<size_t>(InputCodes::Key::_size)> key_down_old{};

			std::array<bool, static_cast<size_t>(InputCodes::Mouse::_size)> mouse_down{};
			std::array<bool, static_cast<size_t>(InputCodes::Mouse::_size)> mouse_down_old{};


			struct /* mouse */ {
				int32_t x = 0;
				int32_t y = 0;
				int32_t dx = 0;
				int32_t dy = 0;

				struct /* wheel */ {
					int32_t dx = 0;
					int32_t dy = 0;
				} wheel;
			} mouse;
	};


};