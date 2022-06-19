#pragma once


namespace Phoenix{
	class NAML_Node{
		public:
			NAML_Node() = default;
			NAML_Node(std::string value) : _value(value) {};
			~NAML_Node();


			void add(std::string key, NAML_Node* node);

			NAML_Node* get(const std::string& key);
			bool has(const std::string& key);

			void forEach(std::function<void(std::string, NAML_Node*)> func);


			//////////////////////////////////////////////////////////////////////
			// value

			template<typename T>
			auto value(){
				static_assert(sizeof(T) == 0, "NAML doesn't support this type");
			};

			template<>
			auto value<std::string>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				return _value;
			};


			template<>
			auto value<float>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				check_float(_value, 1);
				return std::stof(_value);
			}


			template<>
			auto value<uint64_t>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");

				check_float(_value, 0);

				return std::stoull(_value);
			};


			template<>
			auto value<glm::vec3>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				std::vector<std::string> strings = stringDelimiter(_value, ", ");

				check_float(strings[0], 1);
				check_float(strings[1], 1);
				check_float(strings[2], 1);

				return glm::vec3(std::stof(strings[0]), std::stof(strings[1]), std::stof(strings[2]));
			};

			template<>
			auto value<glm::vec4>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				std::vector<std::string> strings = stringDelimiter(_value, ", ");

				check_float(strings[0], 1);
				check_float(strings[1], 1);
				check_float(strings[2], 1);
				check_float(strings[3], 1);

				return glm::vec4(std::stof(strings[0]), std::stof(strings[1]), std::stof(strings[2]), std::stof(strings[3]));
			};


			template<>
			auto value<bool>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				if(_value == "true"){
					return true;
				}else if(_value == "false"){
					return false;
				}else{
					PH_ERROR("Attempted to make a non-boolean into a boolean");
					return false;
				};
			};

			template<>
			auto value<std::vector<std::string>>(){
				PH_THROW(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				std::vector<std::string> strings = stringDelimiter(_value, ", ");

				return strings;
			};

		protected:
			std::map<std::string, NAML_Node*> _nodes;
			std::string _value;


		private:
			void check_float(const std::string& string, int period_count){
				int periods = 0;
				int chars = 1;
				for(auto& character : string){
					switch(character){
						case '0': break;
						case '1': break;
						case '2': break;
						case '3': break;
						case '4': break;
						case '5': break;
						case '6': break;
						case '7': break;
						case '8': break;
						case '9': break;
						case '.':
							periods += 1;
							break;
						case ' ': 
							if(chars == 1){
								break;
							}
						default:
							std::stringstream ss{};
							ss << "Expected a float, got (" << character << ")\n\tfound in: " << string << create_pointer_string(chars);
							throw ss.str();
					}
					
					chars += 1;
				}

				if(periods != period_count){
					std::stringstream ss{};
					ss << "Expected 1 perid, got (" << periods << ")\n\tfound in: " << string;
					throw ss.str();
				}
			}

			std::string create_pointer_string(int chars){
				std::stringstream ss{};

				ss << "\n\t         ";
				for(int i = 0; i < chars; i += 1){
					ss << " ";
				}
				ss << "^";

				return ss.str();
			}
	};



	class NAML_DE{
		public:
			NAML_DE(const std::string& string);
			~NAML_DE();

			inline NAML_Node* get() const { return _head; }
	
		private:
			NAML_Node* _head;
			int _indentation;
	};


}
