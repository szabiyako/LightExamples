#include "Console.h"

std::string UI::Console::consoleText = "";
int UI::Console::linesToDelete = -27;
int UI::Console::symbol = 0;

UI::Console::Console(bool& isOpen)
	: m_isOpen(isOpen)
{
	//m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	//m_windowFlags |= ImGuiWindowFlags_NoScrollbar;
	////m_windowFlags |= ImGuiWindowFlags_MenuBar;
	//m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoResize;
	m_windowFlags |= ImGuiWindowFlags_NoCollapse;
	//m_windowFlags |= ImGuiWindowFlags_NoNav;
	//m_windowFlags |= ImGuiWindowFlags_NoBackground;
	//m_windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void UI::Console::process(const int windowWidth, const int windowHeight, bool &enableKeysInput)
{
	if (!m_isOpen)
		return;
	ImGui::SetNextWindowPos(ImVec2(windowWidth - 400, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Always);
	ImGui::Begin("Console", &m_isOpen, m_windowFlags);
	ImGui::Text(consoleText.c_str());
	ImGui::End();
}

void UI::Console::print(const std::string &message)
{
	//28 lines max
	//55 symbols per line
	std::string textToPrint = message;
	for (size_t i = 0; i < textToPrint.size(); ++i) {
		if (textToPrint[i] == '\n') {
			++linesToDelete;
			symbol = 0;
		}
		else if (symbol > 54) {
			textToPrint.insert(textToPrint.begin() + (i++), '\n');
			++linesToDelete;
			symbol = 1;
		}
		else
			++symbol;
	}
	consoleText += textToPrint;
	cropToFit();
}

void UI::Console::cropToFit()
{
	if (linesToDelete <= 0)
		return;

	while (linesToDelete > 0) {
		std::size_t endlPos = consoleText.find('\n');
		consoleText.erase(0, endlPos + 1);
		--linesToDelete;
	}
}
