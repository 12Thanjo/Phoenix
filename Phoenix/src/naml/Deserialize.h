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
				static_assert(sizeof(T) == 0, "BAD");
			};

			template<>
			auto value<std::string>(){
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				return _value;
			};


			template<>
			auto value<float>(){
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				return std::stof(_value);
			}


			template<>
			auto value<uint64_t>(){
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				return std::stoull(_value);
			};


			template<>
			auto value<glm::vec3>(){
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				std::vector<std::string> strings = stringDelimiter(_value, ", ");

				return glm::vec3(std::stof(strings[0]), std::stof(strings[1]), std::stof(strings[2]));
			};

			template<>
			auto value<glm::vec4>(){
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				std::vector<std::string> strings = stringDelimiter(_value, ", ");

				return glm::vec4(std::stof(strings[0]), std::stof(strings[1]), std::stof(strings[2]), std::stof(strings[3]));
			};


			template<>
			auto value<bool>(){
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
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
				PH_ASSERT(_nodes.size() == 0, "Attempted to get a value of a NAML group");
				
				std::vector<std::string> strings = stringDelimiter(_value, ", ");

				return strings;
			};

		protected:
			std::map<std::string, NAML_Node*> _nodes;
			std::string _value;
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
