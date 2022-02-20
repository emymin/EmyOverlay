#pragma once
#include <iostream>

enum class TextColor :int {
	GREEN = 10,
	RED = 12,
	YELLOW = 14,
	DEFAULT = 15,

};

class Console
{
private:
	static void print(std::string text, TextColor color = TextColor::DEFAULT);
public:
	static void Log(std::string text);
	static void Warning(std::string text);
	static void Error(std::string text);

};