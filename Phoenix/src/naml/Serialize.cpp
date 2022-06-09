#include "ph_pch.h"
#include "Serialize.h"


namespace Phoenix{

	void NAML_S::indent(){
		for(int i = 0; i < _indentation; i += 1){
			_text << "\t";
		}
	}


	void NAML_S::newline(){
		if(!_first){
			_text << "\n";
		}else{
			_first = false;
		};
	}

	void NAML_S::begin(){
		newline();
		indent();
	}

	/////////////////////////////////////////////////////////
	// key / value
	void NAML_S::keyValue(std::string key, bool value){
		begin();
		_text << "-" << key << ": " << (value ? "true" : "false");
	}
	void NAML_S::keyValue(std::string key, float value){
		begin();
		_text << "-" << key << ": " << std::to_string(value);
	}
	
	void NAML_S::keyValue(std::string key, std::string value){
		begin();
		_text << "-" << key << ": " << value;
	}
	void NAML_S::keyValue(std::string key, const char* value){
		begin();
		_text << "-" << key << ": " << value;
	}

	/////////////////////////////////////////////////////////
	// group
	void NAML_S::beginGroup(std::string key){
		begin();
		_text << ">" << key << ":";
		_indentation += 1;
	}

	void NAML_S::endGroup(){
		_indentation -= 1;
	}


	/////////////////////////////////////////////////////////
	// list 
	void NAML_S::beginList(std::string key){
		begin();
		_text << "~" << key << ": ";
		_just_began_list = true;
	}

	void NAML_S::add_to_list(std::string value){
		if(_just_began_list){
			_just_began_list = false;
		}else{
			_text << ", ";
		};

		_text << value;
	}

	void NAML_S::addToList(std::string value){
		add_to_list(value);
	}
	void NAML_S::addToList(const char* value){
		add_to_list(std::string(value));
	}
	void NAML_S::addToList(float value){
		add_to_list(std::to_string(value));
	}
	void NAML_S::addToList(bool value){
		add_to_list(value ? "true" : "false");
	}


	void NAML_S::endList(){
		// _text << "\n";
	}

}
