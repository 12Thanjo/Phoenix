#pragma once


namespace Phoenix{
	static std::vector<std::string> stringDelimiter(const std::string& string, const std::string& delimiter){
			std::vector<std::string> strings;

		    size_t pos = 0;
		    size_t prev = 0;
		    while((pos = string.find(delimiter, prev)) != std::string::npos){
		        strings.push_back(string.substr(prev, pos - prev));
		        prev = pos + 1;
		    }

		    // To get the last substring (or only, if delimiter is not found)
		    strings.push_back(string.substr(prev));


		    return strings;
	}
}
