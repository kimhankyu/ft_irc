#include "Util.h"
#include <sstream>
#include <vector>


std::vector<std::string> ft_split(std::string str, char delim) {
	std::stringstream ss(str);
	std::string buffer;
	std::vector<std::string> ret;

	while (getline(ss, buffer, delim)) {
		ret.push_back(buffer);
	}
	return ret;
}
