#pragma once


namespace ph{
	namespace InputCodes{
	
		enum class Mouse{
			Left    = 1,
			Right   = 2,
			Middle  = 3,
			Back    = 4,
			Forward = 5,

			_size,
		};

		// I've formatted the keycodes to as close to a standard ANSI layout keyboard as possible
		// Do not rely on these being specific values - there is no guarantee that they will not change
		enum class Key{
			Unknown,																																	      
																																								 // PlayPause, StopMedia, PreviousTrack, NextTrack,
			// 																																			      		        VolumeMute, VolumeUp, VolumeDown,
			Escape, 	  F1,    F2,    F3,    F4,            F5,   F6,    F7,    F8,            F9,   F10,    F11,    F12,		PrintScreen, ScrollLock, PauseBreak,
			Backtick, Num1,  Num2,  Num3,  Num4,  Num5,  Num6,  Num7,  Num8,  Num9,  Num0,  Dash,   Equal,       Backspace,     Insert,      Home,       PageUp,		NumLock,  Divide, Multiply,     Subtract,
			Tab,	      Q,     W,     E,     R,     T,     Y,     U,     I,     O,    P,   LBracket, RBracket, Backslash,     Delete,      End,        PageDown,      Numpad7, Numpad8,  Numpad9,          Add,
			CapsLock,        A,     S,     D,     F,     G,     H,     J,     K,     L,   Semicolon,  Apostrophe,    Enter,                                             Numpad4, Numpad5,  Numpad6,         //Add
			LShift,             Z,     X,     C,     V,     B,    N,      M,   Comma,  Period,   ForwardSlash,      RShift,                  UpArrow,                   Numpad1, Numpad2,  Numpad3,  NumpadEnter, 
			LControl,    LMeta,   LAlt,                   Spacebar,                 RAlt,  RMeta,  Applications,  RControl,		LeftArrow,	 DownArrow,  RightArrow,	Numpad0,      NumpadPeriod, //NumpadEnter


			_size,
		};

	};
};