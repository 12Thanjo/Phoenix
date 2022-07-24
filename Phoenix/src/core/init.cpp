#include "ph_pch.h"

// #include <jxcore.h>
#include <windows.h>
#include "Engine.h"

namespace Phoenix{
	
	int main(int argc, char** args){
		Phoenix::_logging::init();

		// jxcore::init(args[0]);
		
		Phoenix::Engine* engine = Phoenix::init( std::string(args[0]) );
		engine->run();
		
		// jxcore::close();
		delete engine;

		return 0;
	}

}

#ifdef PH_DEBUG

int main(int argc, char** args){
	
	return Phoenix::main(argc, args);

}

#else


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow){

	return Phoenix::main(__argc, __argv);
}


#endif