#include "CMD.h"


#include "ph_pch.h"

namespace Phoenix::CMD{
	std::string get_time(){
		time_t current_time;
		struct tm  local_time;

		time ( &current_time );
		localtime_s(&local_time, &current_time);


		int hours = local_time.tm_hour;
		int minutes = local_time.tm_min;
		int seconds = local_time.tm_sec;

		std::string output = "[";

		if(hours < 10){
			output += "0";
		}
		output += std::to_string(hours) + ":";

		if(minutes < 10){
			output += "0";
		}
		output += std::to_string(minutes) + ":";

		if(seconds < 10){
			output += "0";
		}


		return output + std::to_string(seconds) + "]";
	}
}

