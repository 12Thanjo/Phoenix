#include "ph_pch.h"

#include <jxcore.h>


#include "Engine.h"

// #include <lib/lemon/engine/Core.h>


// int main(int argc, char* argv[]){
int main(int argc, char **args){
	Phoenix::_logging::init();

	// Lime::init(argc, argv);
	jxcore::init(args[0]);

	
	Phoenix::Engine* engine = Phoenix::init();
	engine->run();
	
	jxcore::close();
	delete engine;

	return 0;
}