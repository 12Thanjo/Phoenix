// #include "stdafx.h"

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

#define flush_console(...)        \
	do {                            \
		fprintf(stdout, __VA_ARGS__); \
		fflush(stdout);               \
				} while (0)


#include <windows.h>


namespace jxcore{

	// static JXValue eventCB = {0};
	
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



	void sampleMethod(JXResult *results, int argc) {
		flush_console("sampleMethod Called;\n");

		std::stringstream ss_result;
		for (int i = 0; i < argc; i++) {
			std::string str_result;
			ConvertResult(&results[i], str_result);
			ss_result << i << " : ";
			ss_result << str_result << "\n";
		}

		flush_console("%s", ss_result.str().c_str());

		// return an Array back to JS Land
		const char *str = "[1, 2, 3]";

		// results[argc] corresponds to return value
		JX_SetJSON(&results[argc], str, (const int32_t)strlen(str));
	}


	void test_func(JXResult *results, int argc){
		JX_SetInt32(&results[argc], JX_GetInt32(&results[0]));
	}



	void log(JXResult *results, int argc){
		// std::cout << &results[0] << "\n";

		std::stringstream ss_result;
		for (int i = 0; i < argc; i++) {
			std::string str_result;
			ConvertResult(&results[i], str_result);
			// ss_result << i << " : ";
			// ss_result << str_result << "\n";
			ss_result << str_result;
		}
		// std::cout << "test_func argc: " << str_result << "\n";
		PH_JX_LOG(ss_result.str());

		// JX_SetInt32(&results[argc], 12);
	}

	void warning(JXResult *results, int argc){
		std::stringstream ss_result;
		for (int i = 0; i < argc; i++) {
			std::string str_result;
			ConvertResult(&results[i], str_result);
			ss_result << str_result;
		}

		PH_JX_WARNING(ss_result.str());
	}




	std::string read_file(std::string filepath){
		// std::string filepath = "test.js";
		std::ifstream stream(filepath);

		std::string line;
		std::stringstream ss;

		ss << "try{";

		while(getline(stream, line)){
			ss << line << "\n";
		};

		ss << "}catch(e){\nprocess.natives.warning('\\n', e.stack);\n}";

		return ss.str();
	}



	void run(std::string file, std::string code){

		JXValue result;
		JX_Evaluate(code.c_str(), file.c_str(), &result);


		if(result.type_ == RT_Error){
			PH_WARNING(JX_GetString(&result));
		}

		JX_Free(&result);

	}


	// static void defineEventCB(JXValue *results, int argc) {
	//   if (!JX_IsFunction(results + 1)) {
	//     PH_WARNING("defineEventCB expects a function");
	//     return;
	//   }

	//   JX_MakePersistent(results+1);
	//   eventCB = *(results + 1);
	// }


	void callback(JXResult *results, int argc) {
		warning(results, argc);
	}


	void init(const char* path){

		JX_Initialize(path, callback);

		JX_InitializeNewEngine();

		std::string contents = "process.requireGlobal = require;";
		JX_DefineMainFile(contents.c_str());

		JX_DefineExtension("sampleMethod", sampleMethod);
		JX_DefineExtension("test_func", test_func);
		JX_DefineExtension("log", log);
		JX_DefineExtension("warning", warning);
		// JX_DefineExtension("warning", defineEventCB);


		JX_DefineFile("test.js", "exports.x=4");

		JX_StartEngine();

		// loop for possible IO
		// or JX_Loop() without usleep / while
		while (JX_LoopOnce() != 0) Sleep(1);

		
		// std::string file = read_file("./bin/bin/Debug/windows/Phoenix Editor/test.js");

		run("internal", "try{"
			"const phoenix = process.natives;"
			"process.natives.asyncCallback = undefined; //IDK what this does...\n"
			"const require = process.requireGlobal;"
			"console.log = process.natives.log;"
			// "process.natives = undefined;"
		"}catch(e){process.natives.error('\\n', e.stack);\n}");

		run("internal", "console.log('jxcore!');");


		JXValue cb;
		JXValue test;
		JXValue out;

		JX_Evaluate("(function (x) { console.log('x:', x); })", "", &cb);
		JX_Evaluate(read_file("assets/scripts/test.js").c_str(), "", &test);

		// JXValue foo;
		// const int32_t val = 12;
		// JX_SetInt32(&foo, val);

		JXValue params[1] = { cb };
		// JX_CallFunction(&cb, params, 1, &out);

		JX_CallFunction(&test, params, 1, &out);
		// run("./test.js", file);

		// run("assets/scripts/test.js", read_file("assets/scripts/test.js"));

		// loop for possible IO
		// or JX_Loop() without usleep / while
		while (JX_LoopOnce() != 0) Sleep(1);
		

	}


	void close(){
		JX_StopEngine();
	}

}