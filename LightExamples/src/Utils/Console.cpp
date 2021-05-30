#include "Console.h"

#include <iostream>
#include "UI/Elements/Utils/Console.h"

void Console::print(const std::string& message, const Type& type)
{
	switch (type)
	{
	case Type::OS: printOS(message); return;
	case Type::OpenGL: printOGL(message); return;
	}
}

void Console::printOS(const std::string& message)
{
	std::cout << message << std::flush;
}

void Console::printOGL(const std::string& message)
{
	UI::Console::print(message);
}