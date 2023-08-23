
#include "./game.h"


// #if defined(PH_PLATFORM_WINDOWS) && defined(PH_BUILD_RELEASE)
// 	#include <windows.h>

// 	int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow){
// 		ph::execute();
		
// 		return 0;
// 	}

// #else
	auto main([[maybe_unused]] int argc, [[maybe_unused]] const char* args[]) noexcept -> int {
		
		Game::run();

		return 0;
	}

// #endif