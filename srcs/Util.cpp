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

bool isLetter(char c)
{
	if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return true;
	else
		return false;
}

bool isNumber(char c)
{
	if ('0' <= c && c <= '9')
		return true;
	else
		return false;
}

bool isSpecial(char c)
{
	if (c == '_' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' || c == '{' || c == '}' || c == '|')
		return true;
	else
		return false;
}
