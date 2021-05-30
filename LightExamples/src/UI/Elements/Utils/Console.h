#pragma once

#include "UI/Element.h"
#include "string"

namespace UI {

class Console : public Element
{
public:
	Console(bool& isOpen);
	void process(const int windowWidth, const int windowHeight, bool &enableKeysInput) override;
	static void print(const std::string &message);
private:
	ImGuiWindowFlags m_windowFlags = 0;
	bool& m_isOpen;

	static void cropToFit();
	static std::string consoleText;
	static int linesToDelete;
	static int symbol;
};

}