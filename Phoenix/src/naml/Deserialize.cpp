#include "ph_pch.h"
#include "Deserialize.h"


namespace Phoenix{
	
	///////////////////////////////////////////////////////
	// helper functions

	static enum NodeType{
		None,
		Key,
		Group,
		List,
	} node_type;

	static NodeType get_type(const std::string& string){
		switch(string[0]){
			case '-': 	return NodeType::Key;
			case '>': 	return NodeType::Group;
			case '~': 	return NodeType::List;
		};
		return NodeType::None;
	}


	static int get_indentation(const std::string& string){
		int count = 0;
		while(string[count] == '\t'){
			count += 1;
		};

		return count;
	}

	static std::string remove_indentation(const std::string& string){
		return string.substr(get_indentation(string), string.length());
	}

	// assumes no indentation
	static std::string remove_type(const std::string& string){
		return string.substr(1, string.length());
	}

	// assumes no indentation
	static std::string get_key(const std::string& string){
		std::istringstream iss(string);
		std::string line;
		std::getline(iss, line, ':');


		return line;
	}


	// assumes no indentation
	static std::string get_value(const std::string& string){
		std::istringstream iss(string);
		std::string line;
		while(std::getline(iss, line, ':')){};


		return line.substr(1, line.length());
	}





	///////////////////////////////////////////////////////
	// Nodes

	NAML_Node::~NAML_Node(){
		for(auto& node : _nodes){
			delete node.second;
		}
	};


	void NAML_Node::add(std::string key, NAML_Node* node){
		_nodes[key] = node;
	}


	NAML_Node* NAML_Node::get(const std::string& key){
		PH_THROW(_nodes.size() > 0, "Attempted to get an index of a NAML value (" << key << ") = " << _value);
		PH_THROW(_nodes.contains(key), "Attempted to get non-existent NAML key (" << key << ")");
		return _nodes[key];		
	}

	bool NAML_Node::has(const std::string& key){
		return _nodes.contains(key);
	}



	void NAML_Node::forEach(std::function<void(std::string, NAML_Node*)> func){
		for(auto& node : _nodes){
			func(node.first, node.second);
		}
	}



	///////////////////////////////////////////////////////
	// Deserializer

	NAML_DE::NAML_DE(const std::string& string){
		_head = new NAML_Node();


		std::stringstream ss(string);

		std::vector<NAML_Node*> stack{};
		stack.push_back(_head);

		std::string line;
		while(getline(ss, line)){
			_indentation = get_indentation(line);
			line = remove_indentation(line);

			NodeType type = get_type(line);
			PH_THROW(type != NodeType::None, "Invalid Node Type");

			line = remove_type(line);
			std::string key = get_key(line);


			std::string value;
			if(type != NodeType::Group){
				value = get_value(line);
			}

			while(stack.size() - 1 > _indentation){
				stack.pop_back();
			};


			switch(type){
				case NodeType::Key:
					stack[stack.size() - 1]->add(key, new NAML_Node(value));
					break;
				case NodeType::Group:
					{
						NAML_Node* node = new NAML_Node();
						stack[stack.size() - 1]->add(key, node);
						stack.push_back(node);
					}
					break;
				case NodeType::List:
					stack[stack.size() - 1]->add(key, new NAML_Node(value));
					break;
			};
		}
	}


	NAML_DE::~NAML_DE(){
		delete _head;
	}


}
