


#pragma once


#ifdef PH_PLATFORM_WINDOWS
	int main(int argc, char** argv){
		Phoenix::CMD::init();

		auto app = Phoenix::initialize();
		app->run();

		delete app;

		return 0;
	}
#else
	#error Platforms other than Windows are not supported
#endif




