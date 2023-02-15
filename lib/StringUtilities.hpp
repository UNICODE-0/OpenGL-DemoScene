#pragma once

#include <string>

enum class StringColor
{
	Black = 30,
	Red = 31,
	Green = 32,
	Yellow = 33,
	Blue = 34,
	Magnenta = 35,
	Cyan = 36,
	White = 37
};

enum class StringType
{
	Bold = 1,
	Underline = 4,
	Inverse = 7
};

std::string getFormattedString(std::string str, StringColor color, StringType type);