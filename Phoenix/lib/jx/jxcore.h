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


// #include <windows.h>
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


	// void test_func(JXResult *results, int argc){
	// 	JX_SetInt32(&results[argc], JX_GetInt32(&results[0]));
	// }



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
			ss_result << str_result;
		}

		PH_JX_INFO(ss_result.str());
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



	void run(std::string code){

		JXValue result;
		JX_Evaluate(code.c_str(), "", &result);


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

	typedef void (*JX_CALLBACK)(JXValue *result, int argc);
	void defineExtension(std::string name, std::string js_name, JX_CALLBACK func){
		JX_DefineExtension(name.c_str(), func);
		run(js_name + "=process.natives." + name + ";");
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
			"const entity = {};"
			// "process.natives = undefined;"
		"}catch(e){process.natives.error('\\n', e.stack);\n}");



		JXValue param1; JX_Evaluate("", "", &param1); JX_SetInt32(&param1, 0);
		JXValue param2; JX_Evaluate("", "", &param2); JX_SetInt32(&param2, 0);
		// JXValue param3; JX_Evaluate("", "", &param3); JX_SetInt32(&param3, 0);
		// JXValue param4; JX_Evaluate("", "", &param4); JX_SetInt32(&param4, 0);

		params = { param1, param2/*, param3, param4 */};

		// JX_SetInt32(&params[0], 42);


		// JXValue out;

		// JX_Evaluate(read_file(std::string(path) + "/scripts/internal.js").c_str(), "", &script_controller);
		JX_Evaluate(
			"var _Script=function(){var $this=this;var private={};this.$op={};$this.create=function(){};$this.update=function(){};};var _ScriptManager=function(){var $this=this;var private={};this.$op={};private.scripts={};$this.create_script=function(id,script){private.scripts[id]=script;script.create();console.log('created script of id:',id);};$this.run_update=function(id){try{private.scripts[id].update();}catch(e){console.warning(e.message);};};};var _script_manager=new _ScriptManager();;phoenix.key={};phoenix.key.num_1=1;phoenix.key.num_2=2;phoenix.key.num_3=3;phoenix.key.num_4=4;phoenix.key.num_5=5;phoenix.key.num_6=6;phoenix.key.num_7=7;phoenix.key.num_8=8;phoenix.key.num_9=9;phoenix.key.num_0=10;phoenix.key.a=11;phoenix.key.b=12;phoenix.key.c=13;phoenix.key.d=14;phoenix.key.e=15;phoenix.key.f=16;phoenix.key.g=17;phoenix.key.h=18;phoenix.key.i=19;phoenix.key.j=20;phoenix.key.k=21;phoenix.key.l=22;phoenix.key.m=23;phoenix.key.n=24;phoenix.key.o=25;phoenix.key.p=26;phoenix.key.q=27;phoenix.key.r=28;phoenix.key.s=29;phoenix.key.t=30;phoenix.key.u=31;phoenix.key.v=32;phoenix.key.w=33;phoenix.key.x=34;phoenix.key.y=35;phoenix.key.z=36;phoenix.key.f1=37;phoenix.key.f2=38;phoenix.key.f3=39;phoenix.key.f4=40;phoenix.key.f5=41;phoenix.key.f6=42;phoenix.key.f7=43;phoenix.key.f8=44;phoenix.key.f9=45;phoenix.key.f10=46;phoenix.key.f11=47;phoenix.key.f12=48;phoenix.key.f13=49;phoenix.key.f14=50;phoenix.key.f15=51;phoenix.key.f16=52;phoenix.key.f17=53;phoenix.key.f18=54;phoenix.key.f19=55;phoenix.key.f20=56;phoenix.key.f21=57;phoenix.key.f22=58;phoenix.key.f23=59;phoenix.key.f24=60;phoenix.key.f25=61;phoenix.key.space=62;phoenix.key.apostrophe=63;phoenix.key.comma=64;phoenix.key.minus=65;phoenix.key.period=66;phoenix.key.slash=67;phoenix.key.semicolon=68;phoenix.key.equal=69;phoenix.key.left_bracket=70;phoenix.key.backslash=71;phoenix.key.right_bracket=72;phoenix.key.grave_accent=73;phoenix.key.world_1=74;phoenix.key.world_2=75;phoenix.key.escape=76;phoenix.key.enter=77;phoenix.key.tab=78;phoenix.key.backspace=79;phoenix.key.insert=80;phoenix.key.delete=81;phoenix.key.right=82;phoenix.key.left=83;phoenix.key.down=84;phoenix.key.up=85;phoenix.key.page_up=86;phoenix.key.page_down=87;phoenix.key.home=88;phoenix.key.end=89;phoenix.key.caps_lock=90;phoenix.key.scroll_lock=91;phoenix.key.num_lock=92;phoenix.key.print_screen=93;phoenix.key.pause=94;phoenix.key.numpad_0=95;phoenix.key.numpad_1=96;phoenix.key.numpad_2=97;phoenix.key.numpad_3=98;phoenix.key.numpad_4=99;phoenix.key.numpad_5=100;phoenix.key.numpad_6=101;phoenix.key.numpad_7=102;phoenix.key.numpad_8=103;phoenix.key.numpad_9=104;phoenix.key.numpad_decimal=105;phoenix.key.numpad_divide=106;phoenix.key.numpad_multiply=107;phoenix.key.numpad_subtract=108;phoenix.key.numpad_add=109;phoenix.key.numpad_enter=110;phoenix.key.numpad_equal=111;phoenix.key.left_shift=112;phoenix.key.left_control=113;phoenix.key.left_alt=114;phoenix.key.left_super=115;phoenix.key.right_shift=116;phoenix.key.right_control=117;phoenix.key.right_alt=118;phoenix.key.right_super=119;phoenix.key.menu=120;console.info('Initialized Internal Scripting Framework');exports=function(func,id){if(func==1){}else if(func==2){_script_manager.run_update(id);};};",
			"", &script_controller);


		run("_script_manager.create_script('102885151', new _Script());");

		// PH_LOG(std::string(path) + "/scripts/test.js");

		// JXValue foo;

		// JX_CallFunction(&script_controller, params, 1, &out);

		// run("./script_controller.js", file);

		// loop for possible IO
		// or JX_Loop() without usleep / while
		// while (JX_LoopOnce() != 0) Sleep(1);
		
		// JX_SetInt32(&params[0], 3);
		// JX_CallFunction(&script_controller, &params[0], 1, &out);
	}

	void run_script_controller(int func, std::string script_id){
		// set params
		JX_SetInt32(&params[0], func);
		JX_SetString(&params[1], script_id.c_str(), script_id.length());

		// run
		JXValue out;
		JX_CallFunction(&script_controller, &params[0], 2, &out);	
	}


	void close(){
		JX_StopEngine();
	}

}