#pragma once


namespace Phoenix::Utils{

	// heavier type should be A (for best performance)
	template<typename A, typename B>
	class Bimap{
		public:
			Bimap() = default;
			~Bimap() = default;

			void insert(const A a, const B b){
				_map.insert({a, b});
			};
	
			//////////////////////////////////////////////////////////////////////
			// left

			A getLeft(const B& index){
				for(auto& pair : _map){
					if(pair.second == index){
						return std::get<0>(pair);
					}
				}
			}

			bool hasLeft(const A& index){
				return _map.contains(index);
			}

			//////////////////////////////////////////////////////////////////////
			// right
			
			B getRight(const A& index){
				return _map[index];
			}

			bool hasRight(const B& index){
				for(auto& pair : _map){
					if(pair.second == index){
						return true;
					}
				}

				return false;
			}

			void forEach(std::function<void(A, B)> func){
				for(auto& value : _map){
					func(value.first, value.second);
				}
			}
			
		private:
			std::map<A, B> _map;

	};

}
