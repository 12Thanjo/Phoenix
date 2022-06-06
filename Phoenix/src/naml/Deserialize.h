#pragma once


namespace Phoenix{

	

	class Node{
		public:
			Node() = default;
			Node(std::string value) : _value(value) {};
			~Node();


			void add(std::string key, Node* node);

			Node* get(std::string key);

			std::string value();

		private:
			std::unordered_map<std::string, Node*> _nodes;
			std::string _value;
	};



	class NAML_DE{
		public:
			NAML_DE(const std::string& string);
			~NAML_DE();

			inline Node* get() const { return _head; }
	
		private:
			Node* _head;
			int _indentation;
	};


}
