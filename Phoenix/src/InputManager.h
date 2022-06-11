#pragma once


// keyboard codes
#define PH_KEY_UNKOWN   		0

#define PH_KEY_1        		1
#define PH_KEY_2        		2
#define PH_KEY_3        		3
#define PH_KEY_4        		4
#define PH_KEY_5        		5
#define PH_KEY_6        		6
#define PH_KEY_7        		7
#define PH_KEY_8        		8
#define PH_KEY_9        		9
#define PH_KEY_0        		10

#define PH_KEY_A        		11
#define PH_KEY_B        		12
#define PH_KEY_C        		13
#define PH_KEY_D        		14
#define PH_KEY_E        		15
#define PH_KEY_F        		16
#define PH_KEY_G        		17
#define PH_KEY_H        		18
#define PH_KEY_I        		19
#define PH_KEY_J        		20
#define PH_KEY_K        		21
#define PH_KEY_L        		22
#define PH_KEY_M        		23
#define PH_KEY_N        		24
#define PH_KEY_O        		25
#define PH_KEY_P        		26
#define PH_KEY_Q        		27
#define PH_KEY_R        		28
#define PH_KEY_S        		29
#define PH_KEY_T        		30
#define PH_KEY_U        		31
#define PH_KEY_V        		32
#define PH_KEY_W        		33
#define PH_KEY_X        		34
#define PH_KEY_Y        		35
#define PH_KEY_Z        		36

#define PH_KEY_F1       		37
#define PH_KEY_F2       		38
#define PH_KEY_F3       		39
#define PH_KEY_F4       		40
#define PH_KEY_F5       		41
#define PH_KEY_F6       		42
#define PH_KEY_F7       		43
#define PH_KEY_F8       		44
#define PH_KEY_F9       		45
#define PH_KEY_F10      		46
#define PH_KEY_F11      		47
#define PH_KEY_F12      		48
#define PH_KEY_F13      		49
#define PH_KEY_F14      		50
#define PH_KEY_F15      		51
#define PH_KEY_F16      		52
#define PH_KEY_F17      		53
#define PH_KEY_F18      		54
#define PH_KEY_F19      		55
#define PH_KEY_F20      		56
#define PH_KEY_F21      		57
#define PH_KEY_F22      		58
#define PH_KEY_F23      		59
#define PH_KEY_F24      		60
#define PH_KEY_F25      		61

#define PH_KEY_SPACE    		62
#define PH_KEY_APOSTROPHE       63
#define PH_KEY_COMMA    		64
#define PH_KEY_MINUS    		65
#define PH_KEY_PERIOD   		66
#define PH_KEY_SLASH    		67
#define PH_KEY_SEMICOLON        68
#define PH_KEY_EQUAL    		69
#define PH_KEY_LEFT_BRACKET     70
#define PH_KEY_BACKSLASH        71
#define PH_KEY_RIGHT_BRACKET    72
#define PH_KEY_GRAVE_ACCENT     73
#define PH_KEY_WORLD_1  		74
#define PH_KEY_WORLD_2  		75
#define PH_KEY_ESCAPE   		76
#define PH_KEY_ENTER    		77
#define PH_KEY_TAB      		78
#define PH_KEY_BACKSPACE        79
#define PH_KEY_INSERT   		80
#define PH_KEY_DELETE   		81
#define PH_KEY_RIGHT    		82
#define PH_KEY_LEFT     		83
#define PH_KEY_DOWN     		84
#define PH_KEY_UP       		85
#define PH_KEY_PAGE_UP  		86
#define PH_KEY_PAGE_DOWN        87
#define PH_KEY_HOME     		88
#define PH_KEY_END      		89
#define PH_KEY_CAPS_LOCK        90
#define PH_KEY_SCROLL_LOCK      91
#define PH_KEY_NUM_LOCK         92
#define PH_KEY_PRINT_SCREEN     93
#define PH_KEY_PAUSE    		94

#define PH_KEY_NUMPAD_0         95
#define PH_KEY_NUMPAD_1         96
#define PH_KEY_NUMPAD_2         97
#define PH_KEY_NUMPAD_3         98
#define PH_KEY_NUMPAD_4         99
#define PH_KEY_NUMPAD_5         100
#define PH_KEY_NUMPAD_6         101
#define PH_KEY_NUMPAD_7         102
#define PH_KEY_NUMPAD_8         103
#define PH_KEY_NUMPAD_9         104
#define PH_KEY_NUMPAD_DECIMAL   105
#define PH_KEY_NUMPAD_DIVIDE    106
#define PH_KEY_NUMPAD_MULTIPLY  107
#define PH_KEY_NUMPAD_SUBTRACT  108
#define PH_KEY_NUMPAD_ADD       109
#define PH_KEY_NUMPAD_ENTER     110
#define PH_KEY_NUMPAD_EQUAL     111

#define PH_KEY_LEFT_SHIFT       112
#define PH_KEY_LEFT_CONTROL     113
#define PH_KEY_LEFT_ALT         114
#define PH_KEY_LEFT_SUPER       115

#define PH_KEY_RIGHT_SHIFT      116
#define PH_KEY_RIGHT_CONTROL    117
#define PH_KEY_RIGHT_ALT        118
#define PH_KEY_RIGHT_SUPER      119
#define PH_KEY_MENU				120



// Mouse codes
#define PH_MOUSE_LEFT	 1
#define PH_MOUSE_RIGHT	 2
#define PH_MOUSE_MIDDLE	 3
#define PH_MOUSE_FORWARD 4
#define PH_MOUSE_BACK	 5


namespace Phoenix{
	class Window;

	class InputManager{
		private:
			bool _keys_down[120];
			bool _mouse_buttons_down[5];
	
		public:
			InputManager(Window* window);
			~InputManager() = default;

			inline bool keyDown(int keycode){ return _keys_down[keycode]; }
			inline bool mouseButtonDown(int button){ return _mouse_buttons_down[button]; };


		friend Window;
	};

}
