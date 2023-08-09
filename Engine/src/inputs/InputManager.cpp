#include "InputManager.h"

#include "GLFW.h"
	
// for define values
#define GLFW_INCLUDE_NONE // no OpenGL
#include <GLFW/glfw3.h>



#include "Logging.h"
#include <format>

namespace ph{

	auto convert_keycode(int keycode) noexcept -> InputCodes::Key;

	
	auto InputManager::init(GLFW::Window& window) noexcept -> bool {
		this->key_down.fill(false);
		this->mouse_down.fill(false);

		// mouse button
		window.set_mouse_button_callback([&](int button, int action, [[maybe_unused]] int mods){
			if(button >= static_cast<int>(InputCodes::Mouse::_size)){
				PH_WARNING(std::format("Recieved unknown or unsupported mouse button code ({})", button));
				return;
			}

			if(action == GLFW_PRESS){
				this->mouse_down[button] = true;

			}else if(action == GLFW_RELEASE){
				this->mouse_down[button] = false;

			}else{
				PH_WARNING(std::format("Unknown mouse button action ({})", action));
			}
		});



		// keyboard
		window.set_key_callback([&](int keycode, [[maybe_unused]] int scancode, [[maybe_unused]] int action, [[maybe_unused]] int mods){
			// ignore repeats
			if(action == GLFW_REPEAT) return;

			InputCodes::Key key = convert_keycode(keycode);
			bool pressed = (action == GLFW_PRESS);

			this->key_down[static_cast<int>(key)] = pressed;
		});



		// mouse_position
		window.set_mouse_move_callback([&](double mouse_x, double mouse_y){
			const int32_t x = static_cast<int32_t>(mouse_x);
			const int32_t y = static_cast<int32_t>(mouse_y);

			this->mouse.dx = x - this->mouse.x;
			this->mouse.dy = y - this->mouse.y;
			this->mouse.x = x;
			this->mouse.y = y;
		});








		PH_INFO("Initialized: Input Manager");
		return true;
	};


	auto InputManager::shutdown() noexcept -> void {
		
		PH_INFO("Shutdown: Input Manager");
	};



	auto InputManager::update() noexcept -> void {
		this->mouse.dx = 0;
		this->mouse.dy = 0;

		this->mouse.wheel.dx = 0;
		this->mouse.wheel.dy = 0;
	};






	auto InputManager::is_mouse_down(InputCodes::Mouse button) const noexcept -> bool {
		return this->mouse_down[static_cast<size_t>(button) - 1];
	};


	auto InputManager::is_mouse_up(InputCodes::Mouse button) const noexcept -> bool {
		return (this->mouse_down[static_cast<size_t>(button) - 1] == false);
	};


	auto InputManager::is_key_down(InputCodes::Key key) const noexcept -> bool {
		return this->key_down[static_cast<size_t>(key)];
	};


	auto InputManager::is_key_up(InputCodes::Key key) const noexcept -> bool {
		return (this->key_down[static_cast<size_t>(key)] == false);
	};





	// TODO: finish the keycodes
	auto convert_keycode(int keycode) noexcept -> InputCodes::Key {
		switch(keycode){
			case GLFW_KEY_UNKNOWN: return InputCodes::Key::Unknown;

			case GLFW_KEY_SPACE: return InputCodes::Key::Spacebar;
			case GLFW_KEY_APOSTROPHE: return InputCodes::Key::Apostrophe;
			case GLFW_KEY_COMMA: return InputCodes::Key::Comma;
			case GLFW_KEY_MINUS: return InputCodes::Key::Dash;
			case GLFW_KEY_PERIOD: return InputCodes::Key::Period;
			case GLFW_KEY_SLASH: return InputCodes::Key::ForwardSlash;
			case GLFW_KEY_0: return InputCodes::Key::Num0;
			case GLFW_KEY_1: return InputCodes::Key::Num1;
			case GLFW_KEY_2: return InputCodes::Key::Num2;
			case GLFW_KEY_3: return InputCodes::Key::Num3;
			case GLFW_KEY_4: return InputCodes::Key::Num4;
			case GLFW_KEY_5: return InputCodes::Key::Num5;
			case GLFW_KEY_6: return InputCodes::Key::Num6;
			case GLFW_KEY_7: return InputCodes::Key::Num7;
			case GLFW_KEY_8: return InputCodes::Key::Num8;
			case GLFW_KEY_9: return InputCodes::Key::Num9;
			case GLFW_KEY_SEMICOLON: return InputCodes::Key::Semicolon;
			case GLFW_KEY_EQUAL: return InputCodes::Key::Equal;
			case GLFW_KEY_A: return InputCodes::Key::A;
			case GLFW_KEY_B: return InputCodes::Key::B;
			case GLFW_KEY_C: return InputCodes::Key::C;
			case GLFW_KEY_D: return InputCodes::Key::D;
			case GLFW_KEY_E: return InputCodes::Key::E;
			case GLFW_KEY_F: return InputCodes::Key::F;
			case GLFW_KEY_G: return InputCodes::Key::G;
			case GLFW_KEY_H: return InputCodes::Key::H;
			case GLFW_KEY_I: return InputCodes::Key::I;
			case GLFW_KEY_J: return InputCodes::Key::J;
			case GLFW_KEY_K: return InputCodes::Key::K;
			case GLFW_KEY_L: return InputCodes::Key::L;
			case GLFW_KEY_M: return InputCodes::Key::M;
			case GLFW_KEY_N: return InputCodes::Key::N;
			case GLFW_KEY_O: return InputCodes::Key::O;
			case GLFW_KEY_P: return InputCodes::Key::P;
			case GLFW_KEY_Q: return InputCodes::Key::Q;
			case GLFW_KEY_R: return InputCodes::Key::R;
			case GLFW_KEY_S: return InputCodes::Key::S;
			case GLFW_KEY_T: return InputCodes::Key::T;
			case GLFW_KEY_U: return InputCodes::Key::U;
			case GLFW_KEY_V: return InputCodes::Key::V;
			case GLFW_KEY_W: return InputCodes::Key::W;
			case GLFW_KEY_X: return InputCodes::Key::X;
			case GLFW_KEY_Y: return InputCodes::Key::Y;
			case GLFW_KEY_Z: return InputCodes::Key::Z;
			case GLFW_KEY_LEFT_BRACKET: return InputCodes::Key::LBracket;
			case GLFW_KEY_BACKSLASH: return InputCodes::Key::Backslash;
			case GLFW_KEY_RIGHT_BRACKET: return InputCodes::Key::RBracket;
			case GLFW_KEY_GRAVE_ACCENT: return InputCodes::Key::Backtick;
			// case GLFW_KEY_WORLD_1:   161 /* non-US #1 */
			// case GLFW_KEY_WORLD_2:   162 /* non-US #2 */
			case GLFW_KEY_ESCAPE: return InputCodes::Key::Escape;
			case GLFW_KEY_ENTER: return InputCodes::Key::Enter;
			case GLFW_KEY_TAB: return InputCodes::Key::Tab;
			case GLFW_KEY_BACKSPACE: return InputCodes::Key::Backspace;
			case GLFW_KEY_INSERT: return InputCodes::Key::Insert;
			case GLFW_KEY_DELETE: return InputCodes::Key::Delete;
			case GLFW_KEY_RIGHT: return InputCodes::Key::RightArrow;
			case GLFW_KEY_LEFT: return InputCodes::Key::LeftArrow;
			case GLFW_KEY_DOWN: return InputCodes::Key::DownArrow;
			case GLFW_KEY_UP: return InputCodes::Key::UpArrow;
			case GLFW_KEY_PAGE_UP: return InputCodes::Key::PageUp;
			case GLFW_KEY_PAGE_DOWN: return InputCodes::Key::PageDown;
			case GLFW_KEY_HOME: return InputCodes::Key::Home;
			case GLFW_KEY_END: return InputCodes::Key::End;
			case GLFW_KEY_CAPS_LOCK: return InputCodes::Key::CapsLock;
			case GLFW_KEY_SCROLL_LOCK: return InputCodes::Key::ScrollLock;
			case GLFW_KEY_NUM_LOCK: return InputCodes::Key::NumLock;
			case GLFW_KEY_PRINT_SCREEN: return InputCodes::Key::PrintScreen;
			case GLFW_KEY_PAUSE:  return InputCodes::Key::PauseBreak;
			case GLFW_KEY_F1: return InputCodes::Key::F1;
			case GLFW_KEY_F2: return InputCodes::Key::F2;
			case GLFW_KEY_F3: return InputCodes::Key::F3;
			case GLFW_KEY_F4: return InputCodes::Key::F4;
			case GLFW_KEY_F5: return InputCodes::Key::F5;
			case GLFW_KEY_F6: return InputCodes::Key::F6;
			case GLFW_KEY_F7: return InputCodes::Key::F7;
			case GLFW_KEY_F8: return InputCodes::Key::F8;
			case GLFW_KEY_F9: return InputCodes::Key::F9;
			case GLFW_KEY_F10: return InputCodes::Key::F10;
			case GLFW_KEY_F11: return InputCodes::Key::F11;
			case GLFW_KEY_F12: return InputCodes::Key::F12;
			// case GLFW_KEY_F13:   302
			// case GLFW_KEY_F14:   303
			// case GLFW_KEY_F15:   304
			// case GLFW_KEY_F16:   305
			// case GLFW_KEY_F17:   306
			// case GLFW_KEY_F18:   307
			// case GLFW_KEY_F19:   308
			// case GLFW_KEY_F20:   309
			// case GLFW_KEY_F21:   310
			// case GLFW_KEY_F22:   311
			// case GLFW_KEY_F23:   312
			// case GLFW_KEY_F24:   313
			// case GLFW_KEY_F25:   314w
			case GLFW_KEY_KP_0: return InputCodes::Key::Numpad0;
			case GLFW_KEY_KP_1: return InputCodes::Key::Numpad1;
			case GLFW_KEY_KP_2: return InputCodes::Key::Numpad2;
			case GLFW_KEY_KP_3: return InputCodes::Key::Numpad3;
			case GLFW_KEY_KP_4: return InputCodes::Key::Numpad4;
			case GLFW_KEY_KP_5: return InputCodes::Key::Numpad5;
			case GLFW_KEY_KP_6: return InputCodes::Key::Numpad6;
			case GLFW_KEY_KP_7: return InputCodes::Key::Numpad7;
			case GLFW_KEY_KP_8: return InputCodes::Key::Numpad8;
			case GLFW_KEY_KP_9: return InputCodes::Key::Numpad9;
			case GLFW_KEY_KP_DECIMAL: return InputCodes::Key::NumpadPeriod;
			case GLFW_KEY_KP_DIVIDE: return InputCodes::Key::Divide;
			case GLFW_KEY_KP_MULTIPLY: return InputCodes::Key::Multiply;
			case GLFW_KEY_KP_SUBTRACT: return InputCodes::Key::Subtract;
			case GLFW_KEY_KP_ADD: return InputCodes::Key::Add;
			case GLFW_KEY_KP_ENTER: return InputCodes::Key::NumpadEnter;
			// case GLFW_KEY_KP_EQUAL:   336
			case GLFW_KEY_LEFT_SHIFT: return InputCodes::Key::LShift;
			case GLFW_KEY_LEFT_CONTROL: return InputCodes::Key::LControl;
			case GLFW_KEY_LEFT_ALT: return InputCodes::Key::LAlt;
			case GLFW_KEY_LEFT_SUPER: return InputCodes::Key::LMeta;
			case GLFW_KEY_RIGHT_SHIFT: return InputCodes::Key::RShift;
			case GLFW_KEY_RIGHT_CONTROL: return InputCodes::Key::RControl;
			case GLFW_KEY_RIGHT_ALT: return InputCodes::Key::RAlt;
			case GLFW_KEY_RIGHT_SUPER: return InputCodes::Key::RMeta;
			case GLFW_KEY_MENU: return InputCodes::Key::Applications;
		};


		PH_WARNING(std::format("Recieved Unknown / unsupported keycode ({})", keycode));
		return InputCodes::Key::Unknown;
	};

};