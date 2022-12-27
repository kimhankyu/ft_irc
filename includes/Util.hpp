#ifndef UTIL_HPP
#define UTIL_HPP

#include <vector>
#include <string>

std::vector<std::string> ft_split(std::string str, char delim);
bool isLetter(char c);
bool isNumber(char c);
bool isSpecial(char c);
int convertPort(std::string str);
bool channelCheck(std::string str);
void print_msg(const std::string client, const std::string msg, const int flag);

#endif
