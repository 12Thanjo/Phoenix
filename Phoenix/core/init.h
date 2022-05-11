#pragma once



// #include "logging.h"

int main(int argc, char const *argv[]){
	Phoenix::_logging::init();
	Phoenix::InputManager::init();

	Phoenix::Engine* engine = Phoenix::init();
	engine->run();

	delete engine;

	return 0;
}