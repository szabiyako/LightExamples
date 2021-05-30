#pragma once

#include <string>

class Console
{
public:
	Console() = delete;
	enum class Type
	{
		OS,
		OpenGL
	};

	static void print(const std::string &message, const Type &type = Type::OpenGL);
private:
	static void printOS(const std::string &message);
	static void printOGL(const std::string &message);
};

