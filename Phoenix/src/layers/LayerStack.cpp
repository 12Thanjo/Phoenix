#include "ph_pch.h"


#include "LayerStack.h"


namespace Phoenix{

	LayerStack::LayerStack(){

	}

	LayerStack::~LayerStack(){
		for(Layer* layer : layers){
			delete layer;
		}
	}


	void LayerStack::push_layer(Layer* layer){
		layers.emplace(layers.begin() + layer_insert_location, layer);
	}

	void LayerStack::push_overlay(Layer* overlay){
		layers.emplace(layers.begin(), overlay);
		layer_insert_location += 1;
	}


	void LayerStack::pop_layer(Layer* layer){
		auto it = std::find(layers.begin(), layers.end(), layer);
		if(it != layers.end()){
			layers.erase(it);
		}
	}

	void LayerStack::pop_overlay(Layer* overlay){
		auto it = std::find(layers.begin(), layers.end(), overlay);
		if(it != layers.end()){
			layers.erase(it);
		 	layer_insert_location -= 1;
		}
	}


}

