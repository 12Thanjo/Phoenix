#pragma once


namespace Phoenix{
	
	class NAML_S{
		public:
			NAML_S() = default;
			~NAML_S() = default;
			
			// key / value
			void keyValue(std::string key, std::string value);
			void keyValue(std::string key, const char* value);
			void keyValue(std::string key, float value);
			void keyValue(std::string key, bool value);
			void keyValue(std::string key, const UUID& value);

			// group
			void beginGroup(std::string key);
			void endGroup();

			// list
			void beginList(std::string key);

			void addToList(std::string value);
			void addToList(const char* value);
			void addToList(float value);
			void addToList(bool value);

			void endList();


			inline std::string output() const { return _text.str(); }

		private:
			std::stringstream _text;
			int _indentation = 0;
			bool _first = true;
			bool _just_began_list = false;

		private:
			void indent();
			void newline();
			void begin();
			void add_to_list(std::string value);
	};

}
