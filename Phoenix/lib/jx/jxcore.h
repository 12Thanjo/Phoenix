#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "jx.h"


#ifdef PH_DEBUG
	#define PH_JX_LOG(x) if(Phoenix::_logging::level <= 0){ std::cout << Phoenix::_logging::style.dim << Phoenix::_logging::get_time() << " <JX> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_JX_INFO(x) if(Phoenix::_logging::level <= 1){ std::cout << Phoenix::_logging::color.cyan << Phoenix::_logging::get_time() << " <JX> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_JX_WARNING(x) if(Phoenix::_logging::level <= 2){ std::cout << Phoenix::_logging::color.yellow << Phoenix::_logging::get_time() << " <JX> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_JX_ERROR(x) if(Phoenix::_logging::level <= 3){ std::cout << Phoenix::_logging::color.red << Phoenix::_logging::get_time() << " <JX> " << x << Phoenix::_logging::style.reset << "\n"; }
	#define PH_JX_FATAL(x) std::cout << Phoenix::_logging::color.black << Phoenix::_logging::backgroundColor.red << Phoenix::_logging::get_time() << " <JX> " << x << Phoenix::_logging::style.reset << "\n";
#else
	#define PH_JX_LOG(x)
	#define PH_JX_INFO(x)
	#define PH_JX_WARNING(x)
	#define PH_JX_ERROR(x)
	#define PH_JX_FATAL(x)
#endif



#include "src/scripting/scripting.h"


namespace jxcore{
	
	void ConvertResult(JXValue *result, std::string &to_result) {
		switch (result->type_) {
		case RT_Null:
			to_result = "null";
			break;
		case RT_Undefined:
			to_result = "undefined";
			break;
		case RT_Boolean:
			to_result = JX_GetBoolean(result) ? "true" : "false";
			break;
		case RT_Int32: {
			std::stringstream ss;
			ss << JX_GetInt32(result);
			to_result = ss.str();
		} break;
		case RT_Double: {
			std::stringstream ss;
			ss << JX_GetDouble(result);
			to_result = ss.str();
		} break;
		case RT_Buffer: {
			to_result = JX_GetString(result);
		} break;
		case RT_JSON:
		case RT_String: {
			to_result = JX_GetString(result);
		} break;
		case RT_Error: {
			to_result = JX_GetString(result);
		} break;
		default:
			to_result = "null";
			return;
		}
	}

	//////////////////////////////////////////////////////////////////////
	// conversion

	std::string getString(JXValue* result){
		return JX_GetString(result);
	}

	int getInt(JXValue* result){
		return JX_GetInt32(result);
	}

	double getDouble(JXValue* result){
		return JX_GetDouble(result);
	}


	void setDouble(JXValue* result, double value){
		JX_SetDouble(result, value);
	}


	void setBool(JXValue* result, bool value){
		JX_SetBoolean(result, value);
	}






	void log(JXResult *results, int argc){
		std::stringstream ss_result;
		for (int i = 0; i < argc; i++) {
			std::string str_result;
			ConvertResult(&results[i], str_result);
			ss_result << str_result << " ";
		}
		PH_JX_LOG(ss_result.str());
	}
	void info(JXResult *results, int argc){
		std::stringstream ss_result;
		for (int i = 0; i < argc; i++) {
			std::string str_result;
			ConvertResult(&results[i], str_result);
			ss_result << str_result << " ";
		}

		PH_JX_INFO(ss_result.str());
	}
	void warning(JXResult *results, int argc){
		std::stringstream ss_result;
		for (int i = 0; i < argc; i++) {
			std::string str_result;
			ConvertResult(&results[i], str_result);
			ss_result << str_result << " ";
		}

		PH_JX_WARNING(ss_result.str());
	}




	std::string read_file(std::string filepath){
		// std::string filepath = "test.js";
		std::ifstream stream(filepath);

		std::string line;
		std::stringstream ss;

		ss << "try{";

		// while(getline(stream, line)){
		// 	ss << line << "\n";
		// };

		ss << Files::readFile(filepath);

		ss << "}catch(e){\nprocess.natives.warning('\\n', e.stack);\n}";

		return ss.str();
	}



	void run(std::string code){

		JXValue result;
		JX_Evaluate(code.c_str(), "", &result);


		if(result.type_ == RT_Error){
			PH_WARNING(JX_GetString(&result));
		}

		JX_Free(&result);

	}


	typedef void (*JX_CALLBACK)(JXValue *result, int argc);
	void defineExtension(std::string name, std::string js_name, JX_CALLBACK func){
		JX_DefineExtension(name.c_str(), func);
		run(js_name + "=process.natives." + name + ";");
	}

	void defineExtension(std::string name, JX_CALLBACK func){
		JX_DefineExtension(name.c_str(), func);
	}


	void callback(JXResult *results, int argc){
		warning(results, argc);
	}

	static Phoenix::Scripting* scripting;
	void setScripting(Phoenix::Scripting* scripting_ptr){
		scripting = scripting_ptr;
	}

	static std::vector<JXValue> params;
	static JXValue script_controller;

	void init(const char* path){

		JX_Initialize(path, callback);

		JX_InitializeNewEngine();

		std::string contents = "process.requireGlobal = require;";
		JX_DefineMainFile(contents.c_str());

		JX_DefineExtension("log", log);
		JX_DefineExtension("info", info);
		JX_DefineExtension("warning", warning);


		JX_StartEngine();

		// loop for possible IO
		// or JX_Loop() without usleep / while
		// while (JX_LoopOnce() != 0) Sleep(1);

		run("try{"
			// "const phoenix = process.natives;"
			"process.natives.asyncCallback = undefined;" //IDK what this method does, so why waste the memory?
			"const require = process.requireGlobal;"
			"console.log = process.natives.log;"
			"console.info = process.natives.info;"
			"console.warning = process.natives.warning;"
			"const phoenix = {};"
			// "process.natives = undefined;"
		"}catch(e){process.natives.error('\\n', e.stack);\n}");



		JXValue param1; JX_Evaluate("", "", &param1); JX_SetInt32(&param1, 0);
		JXValue param2; JX_Evaluate("", "", &param2); JX_SetInt32(&param2, 0);
		JXValue param3; JX_Evaluate("", "", &param3); JX_SetInt32(&param3, 0);

		params = {param1, param2, param3};

		JX_Evaluate(read_file("C:\\Users\\andrew\\OneDrive\\programming\\Phoenix Projects\\test\\scripts\\internal.js").c_str(), "", &script_controller);
	}

	void run_script_controller(int func, std::string script_id, uint32_t entt_id){
		// set params
		JX_SetInt32(&params[0], func);
		JX_SetString(&params[1], script_id.c_str(), (const int32_t)script_id.length());
		JX_SetInt32(&params[2], entt_id);

		// run
		JXValue out;
		JX_CallFunction(&script_controller, &params[0], 3, &out);	
	}


	void close(){
		JX_StopEngine();
	}

}