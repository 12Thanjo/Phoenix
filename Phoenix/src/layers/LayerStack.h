#pragma once

#include "Layer.h"

namespace Phoenix{
	class LayerStack{
		private:
			std::vector<Layer*> layers;
			unsigned int layer_insert_location = 0;
	
		public:
			LayerStack();
			~LayerStack();

			void push_layer(Layer* layer);
			void push_overlay(Layer* overlay);

			void pop_layer(Layer* layer);
			void pop_overlay(Layer* overlay);			
			
			std::vector<Layer*>::iterator begin() { return layers.begin(); }
			std::vector<Layer*>::iterator end() { return layers.end(); }
	};
}

