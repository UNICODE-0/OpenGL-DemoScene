#include "StringUtilities.hpp"

std::string getFormattedString(std::string str, StringColor color, StringType type)
{
	std::string openTag = "\033[" + std::to_string((int)type) + ";" + std::to_string((int)color) + "m";
	std::string closeTag = "\033[0m";
	return openTag + str + closeTag;
}