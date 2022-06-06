#include "ph_pch.h"

#include <jxcore.h>

#include "Engine.h"





// int main(int argc, char const *argv[]){
int main(int argc, char **args){
	Phoenix::_logging::init();


	jxcore::init(args[0]);

	
	Phoenix::Engine* engine = Phoenix::init();
	engine->run();
	
	jxcore::close();
	delete engine;

	return 0;
}